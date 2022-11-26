# Integration Tests
Tests are conducted with BluePill (STM32F103)

## Processing times

### Com-Maintenance

- Process State `Set Effect:Good` takes 138 μs
- Process State `Set Color:00,00,00,00` takes 195 μs

``` C++
if (SerialUSB.available()) {
    setBuildInLed(LOW);
    while (SerialUSB.available()) {
        _shell.ConsumeSymbol(SerialUSB.read());
    }
    setBuildInLed(HIGH);
}
```

### Button Event

- Sending Button state `:Btn:H->L` takes 27.5 μs 

``` C++
setBuildInLed(LOW);
_shell.SendButtonEvent(Button::State::Pressed);
setBuildInLed(HIGH);
```

### LED-Pixel-Maintenance

- Processing idle state (still effect) takes 680 μs (28 μs generating pixel data)
- Processing busy state (rotating effect) takes 706 μs (55 μs generating pixel data)

```C++
setDebugPin(HIGH);
const Color *colors = _pixelRing.Tick();
if (pColorOverride != nullptr) {
    Color::Rgbw_t c = pColorOverride->GetColor();
    for (size_t i = 0; i < PixelCount; i++) {
        _strip.setPixelColor(i, c.red, c.green, c.blue, 0);
    }
} else {
    for (size_t i = 0; i < PixelCount; i++) {
        Color::Rgbw_t c = colors[i].GetColor();
        _strip.setPixelColor(i, c.red, c.green, c.blue, 0);
    }
}
_strip.show();
setDebugPin(LOW);
```