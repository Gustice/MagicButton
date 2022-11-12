using System;
using System.Collections.Generic;
using System.Windows.Data;
using System.Globalization;
using System.Windows.Media;
using EvalApplication.Ux.Types;

namespace EvalApplication.Ux.Converter
{
    public class EnumValueToColorConverter : IValueConverter
    {
        Dictionary<Type, List<Color>> _knownColors = new Dictionary<Type, List<Color>>() {
            // Hint, Info, Warning, Error,
            //{typeof(FeedbackState), new List<Color>{ Colors.DarkGray, Colors.LightBlue, Colors.LightSalmon, Colors.IndianRed} },
            // Initial, Connected, Disconnected, FailedToConnect,
            {typeof(ConnectionState), new List<Color>{ Colors.DarkGray, Colors.LightGreen, Colors.LightBlue, Colors.LightSalmon} },
            // Passive, Acitve
            {typeof(Activation), new List<Color>{ Colors.LightBlue, Colors.LightGreen } },
            };

        readonly SolidColorBrush errorBrush = new SolidColorBrush(Colors.Red);

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (!(value is System.Enum enumerator))
                return errorBrush;
            var type = enumerator.GetType();

            if (parameter != null && parameter is string specifigColors)
            {
                var colorsDefinition = specifigColors;

                var colors = colorsDefinition.Split(',');
                if (colors.Length < Enum.GetValues(type).Length)
                    return errorBrush;

                var color = (Color)ColorConverter.ConvertFromString(colors[(int)value]);
                new SolidColorBrush(color);
            }
            if (_knownColors.ContainsKey(type))
            {
                var colors = _knownColors[type];
                return new SolidColorBrush(colors[(int)value]);
            }
            return errorBrush;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
