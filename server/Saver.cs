using System;
using System.IO;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Logging;

namespace server
{
    public interface ISaver
    {
        void SaveRow(DataPayload p);
    }

    class Saver : IDisposable, ISaver
    {
        private const string DIR = "data";
        private readonly string docPath;
        private readonly StreamWriter sw;
        private readonly ILogger<Saver> logger;

        public Saver(ILogger<Saver> logger, IWebHostEnvironment env)
        {
            Directory.CreateDirectory(DIR);

            var runName = $"{DateTime.Now.ToString("hh:mm tt")    }";
            this.docPath = Path.Combine($"{env.ContentRootPath}/{DIR}", $"{runName}.csv");
            this.sw = File.CreateText(docPath);
            this.logger = logger;
        }

        private void WriteLine(string line)
        {
            sw.WriteLine(line);
            sw.Flush();
            logger.LogDebug($"Saved {line} to {docPath}");
        }

        public void SaveRow(DataPayload p)
        {
            Console.WriteLine($"{DateTime.Now}> {p.Id} is: {p?.Value} >> {docPath}");
            WriteLine($"{DateTime.Now},{p.Id},{p.Value}");
        }

        public void Dispose()
        {
            this.sw.Dispose();
        }
    }
}
