#include <fstream>
#include <filesystem>

#include "PodGenerator.hpp"

namespace fs = std::filesystem;

/// Resolve relative path removing .. directory parts
fs::path resolvePath(fs::path from, const fs::path& path) {
    for (auto& f : path) {
        if (f == "..") {
            from = from.parent_path();
        } else {
            from /= f;
        }
    }

    return from;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cerr << "Usage: podgen [schemas...] -t template_dir -o output_dir -c capnp_root_dir" << std::endl;
        return 1;
    }

    std::string templateRoot;
    std::string outputRoot;
    std::string capnpRoot;
    std::vector<std::string> inputs;

    for (int i = 1; i < argc; i++) {
        if (i < argc - 1 && strcmp(argv[i], "-o") == 0) {
            outputRoot = argv[++i];
        } else if (i < argc - 1 && strcmp(argv[i], "-c") == 0) {
            capnpRoot = argv[++i];
        } else if (i < argc - 1 && strcmp(argv[i], "-t") == 0) {
            templateRoot = argv[++i];
        } else {
            inputs.emplace_back(argv[i]);
        }
    }

    auto fs = kj::newDiskFilesystem();
    auto capnpImportPath = fs::path(capnpRoot).is_absolute() 
        ? fs->getRoot().openSubdir(kj::Path::parse(capnpRoot.substr(1)))
        : fs->getCurrent().openSubdir(kj::Path::parse(capnpRoot));
    auto templateImportPath = fs::path(templateRoot).is_absolute() 
        ? fs->getRoot().openSubdir(kj::Path::parse(templateRoot.substr(1)))
        : fs->getCurrent().openSubdir(kj::Path::parse(templateRoot));

    const kj::ReadableDirectory* paths[2] = {capnpImportPath.get(), templateImportPath.get()};
    kj::ArrayPtr<const kj::ReadableDirectory*> importPaths(paths, 2);

    PodGenStream dummy(std::cout, "", {});

    for (auto& input : inputs) {
        fs::path capnpFile(input);
        if (!fs::exists(capnpFile)) {
            std::cout << "capnp file not found: " << capnpFile << std::endl;
            continue;
        }

        if (capnpFile.extension() != ".capnp") {
            std::cout << "not a capnp file: " << capnpFile << std::endl;
            continue;
        }

        std::cout << "parsing " << capnpFile.string() << std::endl;

        auto podConvertSource = outputRoot / capnpFile;
        podConvertSource.replace_extension(".convert.cpp");

        auto podHeader = outputRoot / capnpToPodImport(capnpFile);
        auto podInclude = podHeader.filename();

        auto podConvertHeader = outputRoot / capnpToConvertImport(capnpFile);
        auto podConvertInclude = podConvertHeader.filename();

        SchemaInfo info;

        auto putType = [&info](PodGenStream&, ::capnp::StructSchema schema, ::capnp::Schema parent) {
            info.internalTypesByName.emplace(schema.getProto().getDisplayName(), schema);
            info.internalTypesById.emplace(schema.getProto().getId(), schema);
            info.schemaParentOf.emplace(schema.getProto().getId(), parent.getProto().getId());
        };

        ::capnp::SchemaParser parser;
        info.schema = parser.parseFromDirectory(fs->getCurrent(), kj::Path::parse(capnpFile), importPaths);

        auto namesp = getNamespace(info.schema);
        if (!namesp.empty()) {
            std::cout << "  found namespace " << namesp << std::endl;
        }
        info.importNamespaces.emplace(capnpFile, namesp);

        generateFromSchema(dummy, info.schema, putType);
        auto externalTypes = findExternalTypes(info.schema);
        info.externalTypes = std::move(externalTypes.typeMap);
        info.podHeaders = std::move(externalTypes.podHeaders);
        info.unions = findUnionFields(info.schema);

        auto parentPath = capnpFile.parent_path();

        for (auto& [alias, import] : getImportsFromCapnp(capnpFile)) {
            fs::path p = capnpFile.parent_path();

            if (import[0] == '/') {
                p = import.substr(1);
            } else {
                p = resolvePath(p, import);
            }

            try {
                std::cout << p << std::endl;
                auto importSchema = parser.parseFromDirectory(fs->getCurrent(), kj::Path::parse(p), importPaths);
                auto ns = getNamespace(importSchema);
                std::cout << "  parsed import " << import;
                if (!ns.empty()) {
                    std::cout << " with namespace " << ns;
                }
                std::cout << std::endl;

                auto countTypes = generateFromSchema(dummy, importSchema, putType);
                if (countTypes > 0) {
                    info.importNamespaces.emplace(p.string(), ns);

                    // stick the import's using alias in the map with the same namespace
                    if (!alias.empty()) {
                        info.importAliases.emplace(alias, p.string());
                    }
                }

                externalTypes = findExternalTypes(importSchema);
                info.externalTypes.merge(externalTypes.typeMap);
                info.unions.merge(findUnionFields(importSchema));
            } catch (kj::Exception& e) {
                std::cout << "ignoring import exception: " << e.getDescription().cStr() << std::endl;
            }
        }

        auto generate = [&](const fs::path& tmpl, const fs::path& dest) {
            std::ifstream in(tmpl);
            if (!in.good()) {
                std::cout << "error loading template " << tmpl << ": " << strerror(errno) << std::endl;
                return false;
            }

            fs::create_directories(dest.parent_path());
            std::ofstream out(dest);
            if (!out.good()) {
                std::cout << "error writing to " << dest << ": " << strerror(errno) << std::endl;
                return false;
            }

            std::cout << "  generating file " << dest << std::endl;
            generateFile(in, out, info, capnpFile);
            return true;
        };
        
        // pod header
        fs::path templatePath = templateRoot;
        bool result = generate(templatePath / "pod.hpp.tmpl", podHeader)
                && generate(templatePath / "pod_convert.hpp.tmpl", podConvertHeader)
                && generate(templatePath / "pod_convert.cpp.tmpl", podConvertSource);

        if (!result) {
            return 1;
        }
    }

    return 0;
}
