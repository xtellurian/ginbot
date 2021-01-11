using System;
using System.IO;

namespace server
{
    class Saver : IDisposable
    {
        private readonly string docPath;
        private readonly StreamWriter sw;

        public Saver(string dir, string runName)
        {
            this.docPath = Path.Combine(dir, $"{runName}.csv");
            this.sw = File.CreateText(docPath);
        }

        void WriteLine(string line)
        {
            sw.WriteLine(line);
            sw.Flush();
        }

        public void SaveRow(DataPayload p)
        {
            WriteLine($"{DateTime.Now},{p.Id},{p.Value}");
        }

        public void Dispose()
        {
            this.sw.Dispose();
        }
    }
}
