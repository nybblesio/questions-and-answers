namespace Nybbles.Questions.Contracts {
    public interface ILogService {
        void Info(string format, params object[] values);
    }
}