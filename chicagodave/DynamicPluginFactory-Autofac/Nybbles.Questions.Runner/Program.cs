using System;
using Autofac;
using Nybbles.Questions.Contracts;
using Nybbles.Questions.Services;

namespace Nybbles.Questions.Runner {
    public static class Program {
        public static int Main(string[] args) {
            var builder = new ContainerBuilder();
            builder.RegisterModule(new ServicesModule());
            var container = builder.Build();

            var service = container.Resolve<IArithmeticService>();
            Console.WriteLine(
                "add: 3 + 9   = {0}", 
                service.ExecuteOperation("add", 3, 9));
            Console.WriteLine(
                "mul: 16 * 16 = {0}", 
                service.ExecuteOperation("mul", 16, 16));                
            
            return 0;
        }
    }
}