# M5 Dial Component Installation Scripts

Quick installation methods for Home Assistant ESPHome.

## 🚀 Method 1: One-Line Install (Recommended)

Run this single command in your Home Assistant terminal:

```bash
wget -O - https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/update-m5dial.sh | bash
```

This will:
- ✅ Download all component files
- ✅ Create proper directory structure
- ✅ Backup existing files
- ✅ Optionally download documentation

---

## 📦 Method 2: Download Script First

### Step 1: Download the update script

```bash
cd /config/esphome
wget https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/update-m5dial.sh
chmod +x update-m5dial.sh
```

### Step 2: Run the script

```bash
./update-m5dial.sh
```

---

## 🛠️ Method 3: Manual Download (Individual Files)

If you prefer to download files manually:

```bash
# Create directory
mkdir -p /config/esphome/components/shys_m5_dial
cd /config/esphome/components/shys_m5_dial

# Base URL
BASE_URL="https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/components/shys_m5_dial"

# Download all component files
wget $BASE_URL/globals.h
wget $BASE_URL/ha_api.h
wget $BASE_URL/ha_device.h
wget $BASE_URL/ha_device_climate.h
wget $BASE_URL/ha_device_cover.h
wget $BASE_URL/ha_device_fan.h
wget $BASE_URL/ha_device_light.h
wget $BASE_URL/ha_device_lock.h
wget $BASE_URL/ha_device_mediaplayer.h
wget $BASE_URL/ha_device_mode_climate_current_temperature.h
wget $BASE_URL/ha_device_mode_climate_target_temperature.h
wget $BASE_URL/ha_device_mode_fan_percentage.h
wget $BASE_URL/ha_device_mode_light_brightness.h
wget $BASE_URL/ha_device_mode_light_color.h
wget $BASE_URL/ha_device_mode_light_tunable_white.h
wget $BASE_URL/ha_device_mode_light_on_off.h
wget $BASE_URL/ha_device_mode_percentage.h
wget $BASE_URL/ha_device_number.h
wget $BASE_URL/ha_device_switch.h
wget $BASE_URL/ha_device_timer.h
wget $BASE_URL/m5dial_display.h
wget $BASE_URL/m5dial_eeprom.h
wget $BASE_URL/m5dial_rfid.h
wget $BASE_URL/m5dial_rotary.h
wget $BASE_URL/m5dial_touch.h
wget $BASE_URL/screensaver_clock.h
wget $BASE_URL/settings_menu.h
wget $BASE_URL/shys_m5_dial.h
```

---

## 📄 Download Sample Config & Documentation

```bash
cd /config/esphome

# Sample configuration
wget -O sample-m5dial-config.yaml https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/sample-config.yaml

# Secrets template
wget -O secrets-m5dial.yaml.template https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/secrets.yaml.template

# Documentation
wget -O M5DIAL-COMPILE-GUIDE.md https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/COMPILE-GUIDE.md
```

---

## 🔄 Updating Existing Installation

If you already have the component installed and want to update:

```bash
cd /config/esphome
wget -O - https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/update-m5dial.sh | bash
```

The script automatically backs up your existing files before updating!

---

## 📝 After Installation

1. **Edit your YAML config** at `/config/esphome/your-m5dial.yaml`:

```yaml
external_components:
  - source:
      type: local
      path: components
    components: [shys_m5_dial]
```

2. **Compile and upload**:

```bash
esphome compile your-m5dial.yaml
esphome upload your-m5dial.yaml
```

3. **Access settings on device**:
   - Long press button to enter settings
   - Choose from 4 beautiful themes
   - Adjust display brightness, timeout, rotation
   - Settings persist across reboots!

---

## 🆘 Troubleshooting

### "wget: command not found"

Use `curl` instead:

```bash
curl -sSL https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl/update-m5dial.sh | bash
```

### "Permission denied"

Make script executable:

```bash
chmod +x update-m5dial.sh
```

### "Directory not found"

Create ESPHome directory:

```bash
mkdir -p /config/esphome/components
```

### Build errors after update

Clear build cache:

```bash
rm -rf /data/build/your-m5dial
esphome clean your-m5dial.yaml
esphome compile your-m5dial.yaml
```

---

## ✨ What's New in This Version?

- 🎨 **4 Beautiful Themes**: Modern Blue, Warm Sunset, Cool Mint, Dark Purple
- ⚙️ **Settings Menu**: On-device customization (long press to access)
- ⚡ **Enhanced Rotary**: Better debouncing, no more jumping
- 🖥️ **Smooth Display**: 7x faster refresh, no glitching
- 🔆 **Rotary Wake**: Encoder now wakes sleeping display
- 💾 **EEPROM Storage**: Settings persist across reboots

Happy controlling! 🚀
