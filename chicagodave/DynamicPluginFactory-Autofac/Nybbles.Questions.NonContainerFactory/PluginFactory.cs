using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.NonContainerFactory {
    public class PluginFactory {
        private static readonly ConcurrentDictionary<string, Type> Plugins = new ConcurrentDictionary<string, Type>();

        private PluginFactory() {
            ScanFilesystemForAssemblies();
            ScanLoadedAssemblies();
        }

        public static PluginFactory Instance { get; } = new PluginFactory();

        private static IEnumerable<Type> FindCandidateTypes(Func<Type, bool> predicate) {
            if (predicate == null)
                throw new ArgumentNullException(nameof(predicate));

            foreach (var assembly in AppDomain.CurrentDomain.GetAssemblies())
                if (!assembly.IsDynamic) {
                    Type[] exportedTypes = null;
                    try {
                        exportedTypes = assembly.GetExportedTypes();
                    }
                    catch (ReflectionTypeLoadException e) {
                        exportedTypes = e.Types;
                    }

                    if (exportedTypes != null)
                        foreach (var type in exportedTypes)
                            if (predicate(type))
                                yield return type;
                }
        }

        private static void ScanLoadedAssemblies() {
            foreach (var type in FindCandidateTypes(t => t.FullName.StartsWith("Nybbles."))) {
                var attr = type
                    .GetCustomAttributes(typeof(PluginAttribute))
                    .Cast<PluginAttribute>()
                    .FirstOrDefault();
                if (attr != null) Plugins[attr.Type] = type;
            }
        }

        private void ScanFilesystemForAssemblies() {
            var files = Directory.EnumerateFiles(
                AppDomain.CurrentDomain.BaseDirectory,
                "Nybbles.Questions.Plugins*.dll");
            foreach (var assemblyFile in files)
                try {
                    Assembly.LoadFrom(assemblyFile);
                }
                catch (Exception e) {
                    Console.WriteLine("Error loading plugin assembly: {0}", e.Message);
                }
        }

        public IPlugin GetByTypeName(string name) {
            if (!Plugins.TryGetValue(name, out var type))
                return null;
            return Activator.CreateInstance(type) as IPlugin;
        }
    }
}