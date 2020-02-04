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

        static void Main(string[] args) {
            // Set counter to starting value of 2. This value is decided based on Excel numbering. 
            // Counting starts at 1 and on the first row there is already data so the counter has to start at the second row.
            _counter = 2;
            
            // Create a new instance of Excelmanager with the documents folder as path and data.xlsx as file name.
            _excelManager = new ExcelManager(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\data.xlsx");

            // Add a worksheet to the Excel file.
            _excelManager.AddWorksheet("SensorData");

            // Retrieve the worksheet for later use.
            _worksheet = _excelManager.GetWorksheet("SensorData");

            // Create a row which contains the names of the variables we want to log.
            List<string[]> headerRow = new List<string[]>() {
              new string[] { "Time", "Temperature 1", "Humidity 1", "Temperature 2", "Humidity 2", "Temperature 3", "Humidity 3", "Probe" }
            };

            // Calculate the headerrange so the headerRow array can be loaded in.
            string headerRange = "A1:" + char.ConvertFromUtf32(headerRow[0].Length + 64) + "1";

            // Loadin the headerRow array.
            _worksheet.Cells[headerRange].LoadFromArrays(headerRow);

            // Print some good text.
            Console.WriteLine("===========================================================");
            Console.WriteLine("                   Arduino Serial Reader                   ");
            Console.WriteLine("===========================================================");
            Console.WriteLine(":> Type 'quit' to exit the application. If you close it with ctrl+C then it will not write to a file!");
            Console.WriteLine();

            // Asking to specify the COM port
            Console.WriteLine(":> Please specify the COM port... (COMX)");
            Console.WriteLine("Available Ports:");

            // Make an empty array
            List<string> ports = new List<string>();

            // Read through all the available serial ports, print them and put them in the array.
            foreach (string s in SerialPort.GetPortNames()) {
                Console.WriteLine("   {0}", s);
                ports.Add(s);
            }

            string name = "";

            // Loop until the ports array contains the name specified by the user.
            while (true) {
                name = Console.ReadLine().ToUpper();

                if (!ports.Contains(name)) {
                    Console.WriteLine(":> Please enter a valid port from list.");
                }else {
                    break;
                }
            }

            Console.WriteLine();

            // Ask to specify a baudrate
            Console.WriteLine(":> Please specify the baudrate...");

            // Read input
            int baudrate = int.Parse(Console.ReadLine());

            // Create a reader instance with the COM name and baudrate
            _reader = new SerialReader(name, baudrate);

            Console.WriteLine(":> Now receiving data.");
            Console.WriteLine();

            // Start the serial reader.
            _reader.Begin();

            // Create a timer thread which fires every 5 seconds. The time is based on the speed of the input. With method OnTimer as callback
            Timer timer = new Timer(OnTimer, new AutoResetEvent(false), 0, 5000);

            // Check if the user wants to quit the program.
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

        // Method fires every time the timer thread calls
        private static void OnTimer(object state) {
            // Retrieve the time.
            string time = DateTime.Now.ToString("HH:mm:ss");
            // Retrieve the data from serial port.
            string data = _reader.ReadLine();

            // Checks if the data is valid
            if (data == "") return;

            // Show the data that has been retrieved
            Console.WriteLine(time + ":: " + data);

            // Split the data on the seperator
            string[] values = data.Split('|');

            // Put data in the excelsheet on the corresponding row and column
            _worksheet.Cells["A" + _counter].Value = time;
            _worksheet.Cells["B" + _counter].Value = double.Parse(values[0] == "nan" ? "-99" : values[0]);
            _worksheet.Cells["C" + _counter].Value = double.Parse(values[1] == "nan" ? "-99" : values[1]);
            _worksheet.Cells["D" + _counter].Value = double.Parse(values[2] == "nan" ? "-99" : values[2]);
            _worksheet.Cells["E" + _counter].Value = double.Parse(values[3] == "nan" ? "-99" : values[3]);
            _worksheet.Cells["F" + _counter].Value = double.Parse(values[4] == "nan" ? "-99" : values[4]);
            _worksheet.Cells["G" + _counter].Value = double.Parse(values[5] == "nan" ? "-99" : values[5]);
            _worksheet.Cells["H" + _counter].Value = double.Parse(values[6] == "nan" ? "-99" : values[6]);
            
            _counter++;
        }

    }
}
