using System;

namespace Nybbles.Questions.Contracts {
    [AttributeUsage(AttributeTargets.Class)]
    public class PluginAttribute : Attribute {
        public PluginAttribute(string type) {
            Type = type;
        }

        public string Type { get; set; }
    }
}