using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using Autofac;
using Nybbles.Questions.Contracts;
using Module = Autofac.Module;

namespace Nybbles.Questions.Services {
    public class ServicesModule : Module {
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

        private static void ScanLoadedAssemblies(ContainerBuilder builder) {
            foreach (var type in FindCandidateTypes(t => t.FullName.StartsWith("Nybbles."))) {
                var attr = type
                    .GetCustomAttributes(typeof(PluginAttribute))
                    .Cast<PluginAttribute>()
                    .FirstOrDefault();
                if (attr != null) {
                    builder.RegisterType(type).Named<IPlugin>(attr.Type);
                }
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

        protected override void Load(ContainerBuilder builder) {
            builder.RegisterType<LogService>()
                .As<ILogService>()
                .SingleInstance();
            builder.RegisterType<ArithmeticService>()
                .As<IArithmeticService>()
                .SingleInstance();
            ScanFilesystemForAssemblies();
            ScanLoadedAssemblies(builder);
        }
    }
}