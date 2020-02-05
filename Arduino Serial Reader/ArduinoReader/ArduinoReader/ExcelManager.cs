using OfficeOpenXml;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoReader {
    class ExcelManager {
        
        private string _path;

        private ExcelPackage _excel;

        private FileInfo _file;

        public ExcelManager(string path) {
            _path = path;

            _excel = new ExcelPackage();
            _file = new FileInfo(path);
        }

        public void AddWorksheet(string name) {
            _excel.Workbook.Worksheets.Add(name);
        }

        public ExcelWorksheet GetWorksheet(string name) {
            return _excel.Workbook.Worksheets[name];
        }

        public void SaveFile() {
            _excel.SaveAs(_file);
        }
    }
}
