using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Plugins2 {
    [Plugin("mul")]
    public class MultiplicationPlugin : IPlugin {
        public int ComputeValue(int a, int b) {
            return a * b;
        }
    }
}