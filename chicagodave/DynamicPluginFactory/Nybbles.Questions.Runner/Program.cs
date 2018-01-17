using System;
using Nybbles.Questions.NonContainerFactory;

namespace Nybbles.Questions.Runner {
    public static class Program {
        public static int Main(string[] args) {
            var addPlugin = PluginFactory.Instance.GetByTypeName("add");
            if (addPlugin == null) {
                Console.WriteLine("add plugin not found");
                return 1;
            }
            
            var multiplyPlugin = PluginFactory.Instance.GetByTypeName("mul");
            if (multiplyPlugin == null) {
                Console.WriteLine("multiply plugin not found");
                return 1;
            }

            Console.WriteLine(
                "add 342 + 42 = {0}", addPlugin.ComputeValue(342, 42));
            
            Console.WriteLine(
                "multiply 2 * 16 = {0}", 
                multiplyPlugin.ComputeValue(2, 16));

            return 0;
        }
    }
}