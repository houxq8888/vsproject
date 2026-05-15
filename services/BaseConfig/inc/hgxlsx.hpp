#ifndef HGXLSX_H
#define HGXLSX_H

#include <xlsxio_read.h>
#include <xlsxio_write.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <vector>

namespace HGMACHINE{
  
/*! \class XLSXIOReader
 *  \brief class for reading data from an .xlsx file
 *\details C++ wrapper for xlsxioread_ functions.
 */
class XLSXIOReader
{
 private:
  xlsxioreader handle;
 public:

  /*! \brief XLSXIOReader constructor, opens .xlsx file
   * \param  filename      path of .xlsx file to open
   * \sa     xlsxioread_open()
   */
  XLSXIOReader (const char* filename);

  /*! \brief XLSXIOReader destructor, closes .xlsx file
   * \sa     xlsxioread_close()
   */
  ~XLSXIOReader ();

  /*! \brief opens
   * \param  sheetname     worksheet name (NULL for first sheet)
   * \param  flags         XLSXIOREAD_SKIP_ flag(s) to determine how data is processed
   * \return XLSXIOReaderSheet object or NULL in case of error
   * \sa     xlsxioread_sheet_open()
   */
  class XLSXIOReaderSheet* OpenSheet (const char* sheetname, unsigned int flags);
};



/*! \class XLSXIOReaderSheet
 *  \brief class for reading data from a sheet in an .xlsx file
 *\details C++ wrapper for xlsxioread_sheet_ functions.
 */
class XLSXIOReaderSheet
{
  friend class XLSXIOReader;
 private:
  xlsxioreadersheet sheethandle;
  XLSXIOReaderSheet (xlsxioreadersheet sheet);
  XLSXIOReaderSheet (xlsxioreader xlsxhandle, const char* sheetname, unsigned int flags);
 public:

  /*! \brief XLSXIOReaderSheet, closes sheet
   * \sa     xlsxioread_sheet_close()
   */
  ~XLSXIOReaderSheet ();

  /*! \brief start reading the next row of data
   * \sa     xlsxioread_sheet_next_row()
   * \sa     GetNextCell()
   */
  bool GetNextRow ();

  /*! \brief read the next column cell
   * \return value (caller must free the result) or NULL if no more cells are available in the current row
   * \sa     xlsxioread_sheet_next_cell()
   * \sa     GetNextRow()
   */
  char* GetNextCell ();

  /*! \brief read the next column cell as a dynamically allocated string value
   * \param  value         reference where value will be stored (caller must free the result)
   * \return true if cell data was available, otherwise false
   * \sa     xlsxioread_sheet_next_cell_string()
   * \sa     GetNextRow()
   */
  bool GetNextCellString (char*& value);

  /*! \brief read the next column cell as a string value
   * \param  value         reference where value will be stored
   * \return true if cell data was available, otherwise false
   * \sa     xlsxioread_sheet_next_cell_string()
   * \sa     GetNextRow()
   */
  bool GetNextCellString (std::string& value);

  /*! \brief read the next column cell as an integer value
   * \param  value         reference where value will be stored
   * \return true if cell data was available, otherwise false
   * \sa     xlsxioread_sheet_next_cell_int()
   * \sa     GetNextRow()
   */
  bool GetNextCellInt (int64_t& value);

  /*! \brief read the next column cell as a floating point value
   * \param  value         reference where value will be stored
   * \return true if cell data was available, otherwise false
   * \sa     xlsxioread_sheet_next_cell_float()
   * \sa     GetNextRow()
   */
  bool GetNextCellFloat (double& value);

  /*! \brief read the next column cell as a date/time value
   * \param  value         reference where value will be stored
   * \return true if cell data was available, otherwise false
   * \sa     xlsxioread_sheet_next_cell_datetime()
   * \sa     GetNextRow()
   */
  bool GetNextCellDateTime (time_t& value);

  /*! \brief extraction operators
   * \sa     GetNextCellString()
   * \name   operator>>
   * \{
   */
  XLSXIOReaderSheet& operator >> (char*& value);
  XLSXIOReaderSheet& operator >> (std::string& value);
  XLSXIOReaderSheet& operator >> (int64_t& value);
  XLSXIOReaderSheet& operator >> (double& value);
  //inline XLSXIOReaderSheet& operator >> (time_t& value);
  /*! @} */
};



inline XLSXIOReader::XLSXIOReader (const char* filename)
{
  handle = xlsxioread_open(filename);
}

inline XLSXIOReader::~XLSXIOReader ()
{
  xlsxioread_close(handle);
}

inline class XLSXIOReaderSheet* XLSXIOReader::OpenSheet (const char* sheetname, unsigned int flags)
{
  xlsxioreadersheet sheethandle;
  if ((sheethandle = xlsxioread_sheet_open(handle, sheetname, flags)) == NULL)
    return NULL;
  return new XLSXIOReaderSheet(sheethandle);
}



inline XLSXIOReaderSheet::XLSXIOReaderSheet (xlsxioreadersheet sheet)
: sheethandle(sheet)
{
}

inline XLSXIOReaderSheet::~XLSXIOReaderSheet ()
{
  xlsxioread_sheet_close(sheethandle);
}

inline bool XLSXIOReaderSheet::GetNextRow ()
{
  return (xlsxioread_sheet_next_row(sheethandle) != 0);
}

inline char* XLSXIOReaderSheet::GetNextCell ()
{
  return xlsxioread_sheet_next_cell(sheethandle);
}

inline bool XLSXIOReaderSheet::GetNextCellString (char*& value)
{
  if (!xlsxioread_sheet_next_cell_string(sheethandle, &value)) {
    value = NULL;
    return false;
  }
  return true;
}

inline bool XLSXIOReaderSheet::GetNextCellString (std::string& value)
{
  char* result;
  if (!xlsxioread_sheet_next_cell_string(sheethandle, &result)) {
    value.clear();
    return false;
  }
  value.assign(result);
  free(result);
  return true;
}

inline bool XLSXIOReaderSheet::GetNextCellInt (int64_t& value)
{
  if (!xlsxioread_sheet_next_cell_int(sheethandle, &value)) {
    value = 0;
    return false;
  }
  return true;
}

inline bool XLSXIOReaderSheet::GetNextCellFloat (double& value)
{
  if (!xlsxioread_sheet_next_cell_float(sheethandle, &value)) {
    value = 0;
    return false;
  }
  return true;
}

inline bool XLSXIOReaderSheet::GetNextCellDateTime (time_t& value)
{
  if (!xlsxioread_sheet_next_cell_datetime(sheethandle, &value)) {
    value = 0;
    return false;
  }
  return true;
}

XLSXIOReaderSheet& XLSXIOReaderSheet::operator >> (char*& value)
{
  GetNextCellString(value);
  return *this;
}

XLSXIOReaderSheet& XLSXIOReaderSheet::operator >> (std::string& value)
{
  GetNextCellString(value);
  return *this;
}

XLSXIOReaderSheet& XLSXIOReaderSheet::operator >> (int64_t& value)
{
  GetNextCellInt(value);
  return *this;
}

XLSXIOReaderSheet& XLSXIOReaderSheet::operator >> (double& value)
{
  GetNextCellFloat(value);
  return *this;
}

/*
XLSXIOReaderSheet& XLSXIOReaderSheet::operator >> (time_t& value)
{
  GetNextCellDateTime(value);
  return *this;
}
*/


    /*! \class XLSXIOWriter
 *  \brief class for writing data to an .xlsx file
 *\details C++ wrapper for xlsxiowrite_ functions.
 */
class XLSXIOWriter
{
 private:
  xlsxiowriter handle;
 public:

  /*! \brief XLSXIOWriter constructor, creates and opens .xlsx file
   * \param  filename      path of .xlsx file to open
   * \param  sheetname     name of worksheet
   * \param  detectionrows number of rows to buffer in memory, zero for none, defaults to 5
   * \sa     xlsxiowrite_open()
   */
  XLSXIOWriter (const char* filename, const char* sheetname = NULL, size_t detectionrows = 5);

  /*! \brief XLSXIOWriter destructor, closes .xlsx file
   * \sa     xlsxiowrite_close()
   */
  ~XLSXIOWriter ();

  /*! \brief specify the row height to use for the current and next rows
   * \param  height        row height (in text lines), zero for unspecified
   * Must be called before the first call to any Add method of the current row
   * \sa     xlsxiowrite_set_row_height()
   */
  void SetRowHeight (size_t height = 0);

  /*! \brief add a column cell
   * \param  name          column name
   * \param  width         column width (in characters)
   * Only one row of column names is supported or none.
   * Call for each column, and finish column row by calling NextRow().
   * Must be called before any NextRow() or the AddCell methods.
   * \sa     NextRow()
   */
  void AddColumn (const char* name, int width = 0);

  /*! \brief add a cell with string data
   * \param  value         string value
   * \sa     NextRow()
   */
  void AddCellString (const char* value);

  /*! \brief add a cell with integer data
   * \param  value         integer value
   * \sa     NextRow()
   */
  void AddCellInt (long long value);

  /*! \brief add a cell with floating point data
   * \param  value         floating point value
   * \sa     NextRow()
   */
  void AddCellFloat (double value);

  /*! \brief add a cell with date and time data
   * \param  value         date and time value
   * \sa     NextRow()
   */
  void AddCellDateTime (time_t value);

  /*! \brief insertion operators
   * \sa     AddCellString()
   * \name   operator<<
   * \{
   */
  XLSXIOWriter& operator << (const char* value);
  XLSXIOWriter& operator << (const std::string& value);
  XLSXIOWriter& operator << (int64_t value);
  XLSXIOWriter& operator << (double value);
  //XLSXIOWriter& operator << (time_t value);
  /*! @} */

  /*! \brief mark the end of a row (next cell will start on a new row)
   * \sa     xlsxiowrite_next_row()
   * \sa     AddCellString()
   */
  void NextRow ();
};




inline XLSXIOWriter::XLSXIOWriter (const char* filename, const char* sheetname, size_t detectionrows)
{
  unlink(filename);
  handle = xlsxiowrite_open(filename, sheetname);
  xlsxiowrite_set_detection_rows(handle, detectionrows);
}

inline XLSXIOWriter::~XLSXIOWriter ()
{
  xlsxiowrite_close(handle);
}

inline void XLSXIOWriter::SetRowHeight (size_t height)
{
  xlsxiowrite_set_row_height(handle, height);
}

inline void XLSXIOWriter::AddColumn (const char* name, int width)
{
  xlsxiowrite_add_column(handle, name, width);
}

inline void XLSXIOWriter::AddCellString (const char* value)
{
  xlsxiowrite_add_cell_string(handle, value);
}

inline void XLSXIOWriter::AddCellInt (long long value)
{
  xlsxiowrite_add_cell_int(handle, value);
}

inline void XLSXIOWriter::AddCellFloat (double value)
{
  xlsxiowrite_add_cell_float(handle, value);
}

inline void XLSXIOWriter::AddCellDateTime (time_t value)
{
  xlsxiowrite_add_cell_datetime(handle, value);
}

inline XLSXIOWriter& XLSXIOWriter::operator << (const char* value)
{
  AddCellString(value); return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (const std::string& value)
{
  AddCellString(value.c_str());
  return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (int64_t value)
{
  AddCellInt(value);
  return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (double value)
{
  AddCellFloat(value);
  return *this;
}

/*
inline XLSXIOWriter& XLSXIOWriter::operator << (time_t value)
{
  AddCellDateTime(value);
  return *this;
}
*/

inline void XLSXIOWriter::NextRow ()
{
  xlsxiowrite_next_row(handle);
}



    int readExcel(const std::string& filePath,
        std::vector<std::vector<std::string>>& data);
    int writeExcel(const std::string &filePath,const std::vector<std::vector<std::string>>& data);

}

#endif