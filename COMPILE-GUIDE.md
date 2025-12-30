# M5 Dial ESPHome Compilation Guide
## Modern UI Version

This guide will help you compile and flash the M5 Dial ESPHome firmware with the modern, beautiful UI redesign.

## 🎨 Modern UI Features

The redesigned UI includes:
- **Gradient color schemes** - Beautiful blue→purple→pink gradients
- **Modern dark theme** - Professional dark backgrounds with elevated surfaces
- **Enhanced visualizations** - Glowing arcs, shadows, and depth effects
- **Improved typography** - Better text hierarchy and readability
- **Modern clock screensaver** - Elegant minimalist design with colored hands
- **Beautiful color wheels** - Multi-ring depth effects for RGB and temperature controls
- **Comprehensive settings menu** - On-device customization with theme selection
- **4 Built-in themes** - Modern Blue, Warm Sunset, Cool Mint, Dark Purple
- **Live adjustments** - Change brightness, rotation, timeout without reflashing

---

## 📋 Prerequisites

### 1. Install ESPHome

**Option A: ESPHome Dashboard (Recommended)**
```bash
pip3 install esphome
```

**Option B: Home Assistant Add-on**
- Install the ESPHome add-on from the Home Assistant Add-on Store

### 2. Install Python Dependencies
```bash
pip3 install esphome-dashboard
```

### 3. Hardware Requirements
- M5Stack Dial (ESP32-S3)
- USB-C cable for programming
- Computer running Windows, macOS, or Linux

---

## 🚀 Quick Start

### Step 1: Clone/Download the Repository
```bash
cd ~
git clone https://github.com/your-username/m5-dial-for-esphome.git
cd m5-dial-for-esphome
```

Or if you've already downloaded it, navigate to the directory:
```bash
cd /home/user/m5-dial-for-esphome
```

### Step 2: Create Configuration Files

**Create secrets.yaml:**
```bash
cp secrets.yaml.template secrets.yaml
nano secrets.yaml  # Edit with your credentials
```

Fill in your WiFi credentials and keys:
```yaml
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"
api_encryption_key: "generate-this-key"
ota_password: "your-secure-password"
```

**Generate encryption key:**
```bash
openssl rand -base64 32
```

### Step 3: Customize Configuration

Edit `sample-config.yaml` to match your Home Assistant devices:

```bash
cp sample-config.yaml my-m5-dial.yaml
nano my-m5-dial.yaml
```

Update the devices section with your actual Home Assistant entity IDs:
```yaml
devices:
  lights:
    - entity: "light.your_actual_light"  # Update this
      name: "Living Room"
      modes:
        rgb_mode:
          enable: true
```

### Step 4: Validate Configuration

Check your configuration for errors:
```bash
esphome config my-m5-dial.yaml
```

### Step 5: Compile the Firmware

**Option A: Compile and Upload via USB**
```bash
esphome run my-m5-dial.yaml
```

This will:
1. Download dependencies
2. Compile the firmware
3. Flash to your M5 Dial (if connected)

**Option B: Compile Only (no upload)**
```bash
esphome compile my-m5-dial.yaml
```

**Option C: Generate Binary for Manual Flash**
```bash
esphome compile my-m5-dial.yaml
# Binary will be in: .esphome/build/m5-dial/.pioenvs/m5-dial/firmware.bin
```

### Step 6: Upload to Device

**First-time setup (USB required):**
```bash
esphome run my-m5-dial.yaml --device /dev/ttyUSB0
```

On Windows, use `COM3` or similar instead of `/dev/ttyUSB0`.

**Subsequent updates (Over-The-Air):**
```bash
esphome run my-m5-dial.yaml --device m5-dial.local
```

Or use the IP address:
```bash
esphome run my-m5-dial.yaml --device 192.168.1.XXX
```

---

## 🎛️ Configuration Options

### Display Settings

```yaml
shys_m5_dial:
  # Screen timeout (milliseconds)
  screen_off_time: 30000  # 30 seconds

  # Screensaver: "off" or "clock"
  screensaver: "clock"

  # Display rotation (0-7) - IMPORTANT: Adjust if display is upside down!
  # 0 = 0° (normal), 2 = 180°, 4 = 90° CW, 6 = 270° CW
  # Try different values if your display orientation is wrong
  display_rotate: 0  # Start with 0, adjust as needed

  # Font settings
  font: "default"  # See globals.h for available fonts
  font_factor: 1.0  # Font size multiplier
```

**Display Rotation Values:**
- `0` - 0° rotation (normal orientation)
- `1` - 0° with horizontal mirroring
- `2` - 180° rotation (upside down)
- `3` - 180° with mirroring
- `4` - 90° clockwise
- `5` - 90° clockwise with mirroring
- `6` - 270° clockwise (90° counter-clockwise)
- `7` - 270° clockwise with mirroring

**If your display shows upside down:** Change `display_rotate` to `0` or try `4` or `6` to find the correct orientation for your setup.

### Modern UI Fonts Available

The modern UI supports these fonts (defined in `globals.h`):
- `default` - Custom 16px bitmap font
- `FreeSans12pt7b`, `FreeSans18pt7b`, `FreeSans24pt7b`
- `FreeSansBold12pt7b`, `FreeSansBold18pt7b`, `FreeSansBold24pt7b`
- `DejaVu12`, `DejaVu18`, `DejaVu24`, `DejaVu40`, `DejaVu56`
- `Orbitron_Light_24`, `Orbitron_Light_32`
- And more...

### Interaction Settings

```yaml
shys_m5_dial:
  # Touch and rotary settings
  long_press_duration: 1200  # Long press time (ms)
  rotary_step_width: 10      # Default rotary step
  send_value_delay: 1200     # Delay before sending to HA
  send_value_lock: 3000      # Lock time after sending
```

### Device Types

#### Lights (Modern Progress Arcs & Color Wheels)
```yaml
lights:
  - entity: "light.my_rgb_light"
    name: "RGB Light"
    modes:
      rgb_mode:          # Beautiful color wheel
        enable: true
        rotary_step_width: 5
      dimm_mode:         # Gradient progress arc
        enable: true
        min_brightness: 1
        max_brightness: 100
      white_mode:        # Temperature gradient
        enable: true
        min_kelvin: 2000
        max_kelvin: 6500
```

#### Covers (Gradient Progress)
```yaml
covers:
  - entity: "cover.blinds"
    name: "Blinds"
    modes:
      position_mode:
        rotary_step_width: 5
```

#### Climate (Modern Temperature Display)
```yaml
climates:
  - entity: "climate.hvac"
    name: "HVAC"
    modes:
      temp_mode:
        min_temperature: 16
        max_temperature: 30
```

---

## 🐛 Troubleshooting

### Compilation Errors

**Error: "Cannot find M5Unified library"**
```bash
# Clear build cache
esphome clean my-m5-dial.yaml
# Try again
esphome run my-m5-dial.yaml
```

**Error: "ModernUI namespace not found"**
- Make sure you're using the modern UI branch/files
- Check that `globals.h` contains the `ModernUI` namespace

### Upload Issues

**Cannot detect device on USB:**
```bash
# Linux: Add user to dialout group
sudo usermod -a -G dialout $USER
# Logout and login again

# Check USB connection
ls /dev/ttyUSB*
# or on macOS:
ls /dev/cu.usbserial*
```

**OTA update fails:**
- Ensure device is on the same network
- Check firewall settings
- Verify OTA password matches

### Display Issues

**Screen stays black:**
- Check display rotation setting
- Verify WiFi connection
- Check logs: `esphome logs my-m5-dial.yaml`

**Colors look wrong:**
- The modern UI uses RGB565 color format
- Colors are optimized for the M5 Dial display

**Text too small/large:**
- Adjust `font_factor` parameter
- Try different fonts from globals.h

---

## 📊 Monitoring & Debugging

### View Logs
```bash
# Real-time logs via USB
esphome logs my-m5-dial.yaml --device /dev/ttyUSB0

# Real-time logs via WiFi
esphome logs my-m5-dial.yaml --device m5-dial.local
```

### Web Server
Enable the web server in your config for browser-based monitoring:
```yaml
web_server:
  port: 80
```

Access at: `http://m5-dial.local` or `http://192.168.1.XXX`

---

## 🎨 Customizing the Modern UI

### Modifying Colors

Edit `components/shys_m5_dial/globals.h`:

```cpp
namespace ModernUI {
    // Change primary colors
    static const uint16_t PRIMARY_BLUE = 0x1E9F;
    static const uint16_t PRIMARY_PURPLE = 0x8019;
    // etc...
}
```

### Color Format Converter
RGB565 format calculator:
```python
def rgb_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

# Example: Convert #1E88E5 to RGB565
r, g, b = 0x1E, 0x88, 0xE5
print(hex(rgb_to_rgb565(r, g, b)))  # 0x1E9F
```

---

## 🔄 Updating

### Update ESPHome
```bash
pip3 install --upgrade esphome
```

### Update M5 Dial Firmware
```bash
cd m5-dial-for-esphome
git pull origin claude/redesign-modern-ui-nMOGl
esphome run my-m5-dial.yaml
```

---

## 📚 Additional Resources

- **ESPHome Documentation**: https://esphome.io
- **M5Stack Dial**: https://docs.m5stack.com/en/core/M5Dial
- **Home Assistant Integration**: https://www.home-assistant.io/integrations/esphome/

---

## 💡 Tips & Best Practices

1. **Start Simple**: Begin with 2-3 devices, then add more
2. **Test Locally**: Validate config before flashing
3. **Backup Config**: Keep copies of working configurations
4. **Monitor Logs**: Watch for errors during first boot
5. **WiFi Signal**: Ensure strong WiFi signal for OTA updates
6. **Entity Names**: Use clear, descriptive names for devices
7. **Rotary Steps**: Adjust rotary_step_width for smooth control

---

## ⚙️ Using the Settings Menu

### Accessing Settings

**Long press the button** to enter/exit settings mode. A high-pitched beep confirms entry, low-pitched confirms exit.

### Navigation

- **Rotate dial**: Navigate menu items or adjust values
- **Short press button**: Select item or toggle edit mode
- **Long press button**: Exit settings (from main menu)

### Settings Categories

#### 1. Display Settings
- **Timeout**: Screen off time (5-300 seconds)
- **Brightness**: Display brightness (10-255)
- **Rotation**: Display orientation (0-7)
- **Font Size**: Font scale factor (0.5-3.0)
- **Screensaver**: Enable/disable clock screensaver

#### 2. Themes
Choose from 4 beautiful themes:
- **Modern Blue** (Default): Blue→Purple→Pink gradients
- **Warm Sunset**: Orange→Red→Yellow warm tones
- **Cool Mint**: Cyan→Green→Aqua cool tones
- **Dark Purple**: Purple→Magenta→Pink dark palette

Each theme includes:
- Custom color palette
- Matching backgrounds
- Coordinated text colors
- Preview circles showing colors

#### 3. Future Categories
- **Devices**: Add/remove/modify devices (coming soon)
- **Time**: Time adjustment interface (coming soon)
- **Advanced**: Additional customization (coming soon)

### Editing Values

1. Navigate to a setting (e.g., "Brightness")
2. Press button to enter edit mode (item highlights with accent color)
3. Rotate dial to adjust value (changes are live)
4. Press button again to save and exit edit mode

Settings are automatically saved to EEPROM and persist across reboots!

---

## 🎉 Enjoy Your Modern M5 Dial!

Your M5 Dial now features a beautiful, modern UI with:
- ✨ Stunning gradient progress indicators
- 🎨 Professional color wheels with depth
- 🌡️ Elegant temperature visualizations
- ⏰ Beautiful minimalist clock screensaver
- ⚙️ Comprehensive on-device settings menu
- 🎨 4 beautiful themes to choose from
- 🔧 Live customization without reflashing
- 🎯 Improved readability and user experience
- ⚡ Enhanced rotary encoder responsiveness
- 🖥️ Smooth, glitch-free display updates

Happy controlling! 🚀
