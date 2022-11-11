/**
 * @file Color.h
 * @author Gustice
 * @brief   Color-Class and color Definitions
 * @details Defintion of Color-Class with predefined colors as static resource
 */
#pragma once

#include <stdint.h>
#include <string.h>

/**
 * @brief Color class
 *
 * @details Defines color data and common operations with color objects
 *  \li Supports RGB with overlapping white channel (e.g. SK6812-LED)
 *  \li Supports Operator overloading with '+' and '*' (Combined operation with MixWith)
 *  \li Return formatted data stream that can be directly sent to an LED
 */
class Color {
  public:
    /** @brief Internal RGBW-Color-Type */
    struct Rgbw_t {
        /// @brief Red channel
        uint8_t red;

        /// @brief Green channel
        uint8_t green;

        /// @brief Blue channel
        uint8_t blue;

        /// @brief White channel
        uint8_t white;
    };

    /// @brief Implementation for byte-stream conversion
    typedef void (*fpColor2ByteStream)(Rgbw_t &color, uint8_t raw[sizeof(Rgbw_t)]);

    /** @brief   Construct a new Color object (black by default) */
    Color(void) { SetColor(0, 0, 0, 0); };

    /** @brief   Construct a new Color object according to color structure */
    Color(const Rgbw_t &color) { SetColor(color); };

    /**
     * @brief Construct a new Color object according to rgb color-channel-values
     * @details This constructor also takes an overlapping white channel
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param w White channel (zero by default if not assigned)
     */
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) { SetColor(r, g, b, w); };

    /**
     * @brief Set the Color object according to Rgbw_t structure
     * @param color Color to set
     */
    void SetColor(const Rgbw_t &color) { _color = color; }

    /**
     * @brief Set the Color object according to rgb color-channel values
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param w White channel (zero by default if not assigned)
     */
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) {
        _color.red = r;
        _color.green = g;
        _color.blue = b;
        _color.white = w;
    }

    /**
     * @brief Returns Color as Rgbw_t structure
     * @return Rgbw_t Color
     */
    Rgbw_t GetColor(void) const { return _color; }

    /**
     * @brief Scale color by scaling each color-channel by given factor
     * @param k Scale-factor. uint8_t.Max is full-scale
     * @return Scaled Color
     */
    Color operator*(uint8_t k) const {
        Color c = *this;

        c._color.red = (int)_color.red * k / 0xFF;
        c._color.green = (int)_color.green * k / 0xFF;
        c._color.blue = (int)_color.blue * k / 0xFF;
        c._color.white = (int)_color.white * k / 0xFF;

        return c;
    }

    /**
     * @brief Overlaying two colors my superposing each color channel value
     * @note Colors channels should not overlap since there is no automatic ratio correction.
     *    Use *-Operator first in order to scale each color value a a way that no overflow can occur
     * @param c2 right hand side
     * @return Overlayed Color
     */
    Color operator+(const Color c2) const {
        Color c = *this;

        c._color.red += c2._color.red;
        c._color.green += c2._color.green;
        c._color.blue += c2._color.blue;
        c._color.white += c2._color.white;

        return c;
    }

    /** @brief Checks if both color values are equal */
    bool operator==(Color const &c2) const {
        return memcmp(&_color, &c2._color, sizeof(Rgbw_t)) == 0;
    }

    /** @brief Checks if both color values are not equal */
    bool operator!=(Color const &c2) const {
        return memcmp(&_color, &c2._color, sizeof(Rgbw_t)) != 0;
    }

    /**
     * @brief Scaling and superposing with given factor and color.
     * @param c2 Color 2
     * @param k scaling factor
     * @return Overlayed Color
     */
    Color MixWith(const Color c2, uint8_t k = 0x7F) {
        *this = (*this * (UINT8_MAX - k)) + (c2 * k);
        return *this;
    }

    /**
     * @brief Writes Color as formatted byte-stream to location
     * @param color
     */
    void WriteByteStreamTo(uint8_t raw[sizeof(Rgbw_t)]) {
        raw[0] = _color.blue;
        raw[1] = _color.green;
        raw[2] = _color.red;
        raw[3] = _color.white;
    }

    /** @brief Assign overload */
    Color operator=(Color const *c2) {
        SetColor(c2->_color);
        return *this;
    }

  private:
    fpColor2ByteStream fpColor2Bytes;
    Rgbw_t _color; ///< Color data

    /**
     * @brief Default implementation for byte-converter
     * @param color
     */
    static void DefaultWriteByteStreamTo(Rgbw_t &color, uint8_t raw[sizeof(Rgbw_t)]) {
        raw[0] = color.red;
        raw[1] = color.green;
        raw[2] = color.blue;
        raw[3] = color.white;
    }
};

/// Null color object
extern const Color *noColor;
/// Used old color agreement (if no color pointer)
extern const Color *oldColor;

/// Black color object constant
extern const Color CBlack;
/// Red color object constant
extern const Color CRed;
/// Green color object constant
extern const Color CGreen;
/// Blue color object constant
extern const Color CBlue;
/// Magenta color object constant
extern const Color CMagenta;
/// Cyan color object constant
extern const Color CCyan;
/// Yellow color object constant
extern const Color CYellow;
/// White color object constant
extern const Color CWhite;
/// Warm white color object constant (with higher red channel value)
extern const Color CWarmWhite;
/// Black color object constant (with higher green channel value)
extern const Color CColdWhite;
/// Black color object constant (with higher blue channel value)
extern const Color CSpookyWhite;
