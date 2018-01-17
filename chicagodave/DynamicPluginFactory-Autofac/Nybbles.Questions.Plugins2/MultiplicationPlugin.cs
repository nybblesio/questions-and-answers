using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Plugins2 {
    [Plugin("mul")]
    public class MultiplicationPlugin : IPlugin {
        private readonly ILogService _logger;
        
        public MultiplicationPlugin(ILogService logger) {
            _logger = logger;
        }

        public int ComputeValue(int a, int b) {
            _logger.Info("multiply: a * b");
            return a * b;
        }
    }
}