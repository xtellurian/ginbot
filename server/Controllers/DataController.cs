using System;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace server.Controllers
{
    [Route("/data")]
    public class DataController : Controller
    {
        private readonly ILogger<DataController> logger;
        private readonly ISaver saver;

        public DataController(ILogger<DataController> logger, ISaver saver)
        {
            this.logger = logger;
            this.saver = saver;
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
                logger.LogDebug("No value in request body.");
                return Ok($"{DateTime.Now}> Empty...");
            }
            else
            {
                saver.SaveRow(p);
                return Ok($"You posted to ginbot | {p.Id}:{p?.Value}");
            }
        }
    }
}
