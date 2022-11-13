using System;
using System.Globalization;
using System.Windows.Data;

namespace EvalApplication.Ux.Converter
{
    public class HexByteConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return $"{((byte)value):X2}";
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Convert.ToByte((string)value, 16);
        }
    }
}
