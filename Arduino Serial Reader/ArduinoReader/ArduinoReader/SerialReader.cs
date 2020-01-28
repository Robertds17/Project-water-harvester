using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoReader {
    class SerialReader {
        private SerialPort _port;

        public bool IsOpen { get => _port.IsOpen; }

        public SerialReader(string name, int baudrate) {
            _port = new SerialPort {
                PortName = name,
                BaudRate = baudrate,
                ReadTimeout = 500,
                WriteTimeout = 500
            };
        }

        public string ReadLine() {
            if (!_port.IsOpen) return "";

            try {
                return _port.ReadLine();
            } catch (IOException) { } catch (TimeoutException) { }

            return "";
        }

        public void Begin() {
            if (_port.IsOpen) return;

            _port.Open();
        }

        public void Stop () {
            if (!_port.IsOpen) return;

            _port.Close();
        }
    }
}
