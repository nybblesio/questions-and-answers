using System;
using Nybbles.Questions.Contracts;

namespace Nybbles.Questions.Services {
    public class LogService : ILogService {
        public void Info(string format, params object[] values) {
            Console.WriteLine(format, values);
        }
    }
}