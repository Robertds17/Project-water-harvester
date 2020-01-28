using OfficeOpenXml;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ArduinoReader {
    class Program {
        private static SerialReader _reader;
        private static ExcelManager _excelManager;
        private static ExcelWorksheet _worksheet;

        private static int _counter;

        private static string[] _previousValues;

        static void Main(string[] args) {

            _counter = 2;
            _previousValues = new string[] { "" };

            _excelManager = new ExcelManager(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\data.xlsx");

            _excelManager.AddWorksheet("SensorData");

            _worksheet = _excelManager.GetWorksheet("SensorData");

            List<string[]> headerRow = new List<string[]>() {
              new string[] { "Time", "Temperature 1", "Humidity 1", "Temperature 2", "Humidity 2", "Temperature 3", "Humidity 3", "Probe" }
            };

            string headerRange = "A1:" + char.ConvertFromUtf32(headerRow[0].Length + 64) + "1";

            _worksheet.Cells[headerRange].LoadFromArrays(headerRow);

            Console.WriteLine("===========================================================");
            Console.WriteLine("                   Arduino Serial Reader                   ");
            Console.WriteLine("===========================================================");
            Console.WriteLine(":> Type 'quit' to exit the application. If you close it with ctrl+C then it will not write to a file!");
            Console.WriteLine();

            Console.WriteLine(":> Please specify the COM port... (COMX)");
            Console.WriteLine("Available Ports:");

            List<string> ports = new List<string>();

            foreach (string s in SerialPort.GetPortNames()) {
                Console.WriteLine("   {0}", s);
                ports.Add(s);
            }

            string name = "";

            while (true) {
                name = Console.ReadLine().ToUpper();

                if (!ports.Contains(name)) {
                    Console.WriteLine(":> Please enter a valid port from list.");
                }else {
                    break;
                }
            }

            Console.WriteLine();

            Console.WriteLine(":> Please specify the baudrate...");

            int baudrate = int.Parse(Console.ReadLine());

            _reader = new SerialReader(name, baudrate);

            Console.WriteLine(":> Now receiving data.");
            Console.WriteLine();
            _reader.Begin();

            Timer timer = new Timer(OnTimer, new AutoResetEvent(false), 0, 5000);

            while (true) {
                if (Console.ReadLine().ToLower().Equals("quit")) {
                    _reader.Stop();
                    _excelManager.SaveFile();
                    timer.Dispose();
                    break;
                } else {
                    Console.WriteLine();
                    Console.WriteLine(":> If you meant to quit the application. Type: 'quit'");
                    Console.WriteLine();
                }
            }
        }

        private static void OnTimer(object state) {
            string time = DateTime.Now.ToString("HH:mm:ss");
            string data = _reader.ReadLine();

            if (data == "") return;

            Console.WriteLine(time + ":: " + data);

            string[] values = data.Split('|');

            _worksheet.Cells["A" + _counter].Value = time;
            _worksheet.Cells["B" + _counter].Value = double.Parse(values[0] == "nan" ? "-99" : values[0]);
            _worksheet.Cells["C" + _counter].Value = double.Parse(values[1] == "nan" ? "-99" : values[1]);
            _worksheet.Cells["D" + _counter].Value = double.Parse(values[2] == "nan" ? "-99" : values[2]);
            _worksheet.Cells["E" + _counter].Value = double.Parse(values[3] == "nan" ? "-99" : values[3]);
            _worksheet.Cells["F" + _counter].Value = double.Parse(values[4] == "nan" ? "-99" : values[4]);
            _worksheet.Cells["G" + _counter].Value = double.Parse(values[5] == "nan" ? "-99" : values[5]);
            _worksheet.Cells["H" + _counter].Value = double.Parse(values[6] == "nan" ? "-99" : values[6]);

            _previousValues = values;
            _counter++;
        }

    }
}
