using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Plugins1 {
    [Plugin("add")]
    public class AdditionPlugin : IPlugin {
        private readonly ILogService _logger;

        public AdditionPlugin(ILogService logger) {
            _logger = logger;
        }
        
        public int ComputeValue(int a, int b) {
            _logger.Info("add: a + b");
            return a + b;
        }
    }
}