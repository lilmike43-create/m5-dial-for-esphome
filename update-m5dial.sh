#!/bin/bash

# M5 Dial ESPHome Component Updater
# This script downloads the latest M5 Dial component files to your ESPHome directory

set -e  # Exit on error

# Configuration
GITHUB_RAW_URL="https://raw.githubusercontent.com/lilmike43-create/m5-dial-for-esphome/claude/redesign-modern-ui-nMOGl"
ESPHOME_DIR="/config/esphome"
COMPONENT_DIR="${ESPHOME_DIR}/components/shys_m5_dial"

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}M5 Dial Component Updater${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Check if ESPHome directory exists
if [ ! -d "$ESPHOME_DIR" ]; then
    echo -e "${RED}Error: ESPHome directory not found at $ESPHOME_DIR${NC}"
    echo -e "${YELLOW}Please make sure you're running this on your Home Assistant system${NC}"
    exit 1
fi

# Create component directory structure
echo -e "${YELLOW}Creating directory structure...${NC}"
mkdir -p "$COMPONENT_DIR"

# Backup existing files if they exist
if [ -d "$COMPONENT_DIR" ] && [ "$(ls -A $COMPONENT_DIR)" ]; then
    BACKUP_DIR="${ESPHOME_DIR}/components/shys_m5_dial_backup_$(date +%Y%m%d_%H%M%S)"
    echo -e "${YELLOW}Backing up existing files to: $BACKUP_DIR${NC}"
    mkdir -p "$BACKUP_DIR"
    cp -r "$COMPONENT_DIR"/* "$BACKUP_DIR/" 2>/dev/null || true
fi

# List of component files to download
FILES=(
    "globals.h"
    "ha_api.h"
    "ha_device.h"
    "ha_device_climate.h"
    "ha_device_cover.h"
    "ha_device_fan.h"
    "ha_device_light.h"
    "ha_device_lock.h"
    "ha_device_mediaplayer.h"
    "ha_device_mode_climate_current_temperature.h"
    "ha_device_mode_climate_target_temperature.h"
    "ha_device_mode_fan_percentage.h"
    "ha_device_mode_light_brightness.h"
    "ha_device_mode_light_color.h"
    "ha_device_mode_light_tunable_white.h"
    "ha_device_mode_light_on_off.h"
    "ha_device_mode_percentage.h"
    "ha_device_number.h"
    "ha_device_switch.h"
    "ha_device_timer.h"
    "m5dial_display.h"
    "m5dial_eeprom.h"
    "m5dial_rfid.h"
    "m5dial_rotary.h"
    "m5dial_touch.h"
    "screensaver_clock.h"
    "settings_menu.h"
    "shys_m5_dial.h"
)

# Download each file
echo -e "${YELLOW}Downloading component files...${NC}"
DOWNLOADED=0
FAILED=0

for FILE in "${FILES[@]}"; do
    URL="${GITHUB_RAW_URL}/components/shys_m5_dial/${FILE}"
    DEST="${COMPONENT_DIR}/${FILE}"

    echo -n "  Downloading ${FILE}... "

    if wget -q -O "$DEST" "$URL" 2>/dev/null; then
        echo -e "${GREEN}✓${NC}"
        ((DOWNLOADED++))
    else
        echo -e "${RED}✗ Failed${NC}"
        ((FAILED++))
    fi
done

echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}Download Complete!${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Downloaded: ${GREEN}${DOWNLOADED}${NC} files"
echo -e "Failed: ${RED}${FAILED}${NC} files"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All files downloaded successfully!${NC}"
    echo ""
    echo -e "${YELLOW}Next steps:${NC}"
    echo "1. Update your ESPHome YAML config to use the component"
    echo "2. Run 'esphome compile your-config.yaml' to build"
    echo "3. Flash to your M5 Dial device"
    echo ""
    echo -e "${BLUE}Component location: ${COMPONENT_DIR}${NC}"
else
    echo -e "${RED}⚠ Some files failed to download${NC}"
    echo -e "${YELLOW}Please check your internet connection and try again${NC}"
    exit 1
fi

# Download sample config and documentation (optional)
echo -e "${YELLOW}Would you like to download sample config and documentation? (y/n)${NC}"
read -r DOWNLOAD_DOCS

if [[ "$DOWNLOAD_DOCS" =~ ^[Yy]$ ]]; then
    echo ""
    echo -e "${YELLOW}Downloading documentation...${NC}"

    # Download sample config
    if wget -q -O "${ESPHOME_DIR}/sample-m5dial-config.yaml" "${GITHUB_RAW_URL}/sample-config.yaml"; then
        echo -e "  ${GREEN}✓${NC} sample-m5dial-config.yaml"
    fi

    # Download secrets template
    if wget -q -O "${ESPHOME_DIR}/secrets-m5dial.yaml.template" "${GITHUB_RAW_URL}/secrets.yaml.template"; then
        echo -e "  ${GREEN}✓${NC} secrets-m5dial.yaml.template"
    fi

    # Download compile guide
    if wget -q -O "${ESPHOME_DIR}/M5DIAL-COMPILE-GUIDE.md" "${GITHUB_RAW_URL}/COMPILE-GUIDE.md"; then
        echo -e "  ${GREEN}✓${NC} M5DIAL-COMPILE-GUIDE.md"
    fi

    echo ""
    echo -e "${GREEN}Documentation downloaded to: ${ESPHOME_DIR}${NC}"
fi

echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}Setup Complete! 🎉${NC}"
echo -e "${BLUE}========================================${NC}"
