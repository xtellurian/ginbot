using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using server.Models;

namespace server.Controllers
{
    [Route("/data")]
    public class DataController : Controller
    {
        private readonly ILogger<DataController> _logger;
        private static Saver saver;
        public DataController(ILogger<DataController> logger, IWebHostEnvironment env)
        {
            var run = $"{DateTime.Now.ToShortTimeString()}";
            _logger = logger;
            saver ??= new Saver(env.ContentRootPath, run);
        }

        [HttpGet]
        public IActionResult Get()
        {
            return Ok("Hello from Ginbot Server");
        }

        [HttpPost]
        public IActionResult Post([FromBody] DataPayload p)
        {
            if (p is null)
            {
                Console.WriteLine("No Value");
                return Ok($"{DateTime.Now}> Empty...");
            }
            else
            {
                Console.WriteLine($"{DateTime.Now}> {p.Id} is: {p?.Value}");
                Console.WriteLine("Saving Row...");
                saver.SaveRow(p);
                return Ok($"You posted to ginbot | {p.Id}:{p?.Value}");
            }
        }
    }
}
