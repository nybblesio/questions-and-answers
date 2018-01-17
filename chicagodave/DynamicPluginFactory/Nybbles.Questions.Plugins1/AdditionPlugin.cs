using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Plugins1 {
    [Plugin("add")]
    public class AdditionPlugin : IPlugin {
        public int ComputeValue(int a, int b) {
            return a + b;
        }
    }
}