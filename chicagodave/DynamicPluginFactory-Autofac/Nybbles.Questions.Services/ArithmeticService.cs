using Autofac;
using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Services {
    public class ArithmeticService : IArithmeticService {
        private readonly ILogService _logger;
        private readonly IComponentContext _context;
        
        public ArithmeticService(
                ILogService logger,
                IComponentContext context) {
            _logger = logger;
            _context = context;
        }
        
        public int ExecuteOperation(string name, int a, int b) {
            var op = _context.ResolveNamed<IPlugin>(name);
            if (op != null) {
                return op.ComputeValue(a, b);
            }
            _logger.Info("Unable to resolve the named plugin.");
            return 0;
        }
    }
}