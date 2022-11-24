using System;
using System.Windows.Data;
using System.Globalization;
using System.Windows.Media;

namespace Application.Converter
{
    public class BoolToColorConverter : IValueConverter
    {
        readonly SolidColorBrush errorBrush = new SolidColorBrush(Colors.Red);

        public Color IfTrue { get; set; }
        public Color IfFalse { get; set; }

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (!(value is bool binary))
                return errorBrush;

            var color = IfFalse;
            if (binary)
                color = IfTrue;

            return new SolidColorBrush(color);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
