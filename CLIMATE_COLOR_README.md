# Climate Display Color Customization

This feature allows you to customize all colors on the M5Dial climate control screen directly from your ESPHome YAML configuration, without needing to modify any C++ code.

## Features

- **24 customizable color parameters** covering every visual element
- **Hex color support** using standard web color format (e.g., `0xFF0000` for red)
- **Full backward compatibility** - defaults match the original hardcoded colors
- **Partial customization** - override only the colors you want to change
- **Per-device configuration** - different colors for each climate entity

## Available Color Parameters

### Screen Background
- `screen_bg`: Full display background color (default: `0x000000` - Black)

### Temperature Arc
- `arc_base`: Base arc from start to lowest temperature (default: `0x444444` - Dark grey)
- `arc_heating_active`: Gap color when heating is ON and below setpoint (default: `0xFF0000` - Red)
- `arc_heating_idle`: Gap color when heating is ON but above setpoint (default: `0xFFA500` - Orange)
- `arc_cooling`: Gap color when room is warmer than setpoint (default: `0x0000FF` - Blue)
- `arc_rest`: Rest of arc from highest temperature to end (default: `0xFFA500` - Orange)

### Current Temperature Circle (LEFT)
- `current_temp_bg`: Circle background color (default: `0x444444` - Dark grey)
- `current_temp_outline`: Circle outline color (default: `0xFFFFFF` - White)
- `current_temp_text`: Temperature number and "NOW" label color (default: `0xFFFFFF` - White)

### Setpoint Circle (RIGHT)
- `setpoint_bg`: Circle background color (default: `0xFFA500` - Orange)
- `setpoint_outline`: Circle outline color (default: `0xFFFFFF` - White)
- `setpoint_text`: Temperature number and "SET" label color (default: `0xFFFFFF` - White)

### Arc Marker Dots
- `current_temp_marker`: Small dot on arc showing current temperature (default: `0xFFFFFF` - White)
- `setpoint_marker`: Larger dot on arc showing setpoint (default: `0xFFFF00` - Yellow)

### Action Badge (CENTER)
Four states with separate background and text colors:
- `badge_heating_bg` / `badge_heating_text`: When actively heating (defaults: `0xFF0000` / `0xFFFFFF`)
- `badge_cooling_bg` / `badge_cooling_text`: When actively cooling (defaults: `0x0000FF` / `0xFFFFFF`)
- `badge_idle_bg` / `badge_idle_text`: When HVAC is idle (defaults: `0xFFA500` / `0xFFFFFF`)
- `badge_off_bg` / `badge_off_text`: When HVAC is off (defaults: `0x808080` / `0xFFFFFF`)

### Bottom Bar
- `bottom_bar_bg`: Background color for device name bar (default: `0x000080` - Navy)
- `bottom_bar_text`: Text color for device name and mode (default: `0xFFFFFF` - White)

## Usage Examples

### Example 1: Default Colors (No Customization)
```yaml
shys_m5_dial:
  devices:
    climates:
      - entity: climate.bedroom
        name: "Bedroom"
        modes:
          temp_mode:
            min_temperature: 15
            max_temperature: 30
            rotary_step_width: 0.5
            # No colors section = uses all defaults
```

### Example 2: Full Customization
```yaml
shys_m5_dial:
  devices:
    climates:
      - entity: climate.fireplace
        name: "Fireplace"
        modes:
          temp_mode:
            min_temperature: 15
            max_temperature: 30
            rotary_step_width: 0.5

            colors:
              screen_bg: "0x000000"
              arc_base: "0x444444"
              arc_heating_active: "0xFF0000"
              arc_heating_idle: "0xFFA500"
              arc_cooling: "0x0000FF"
              arc_rest: "0xFFA500"
              current_temp_bg: "0x444444"
              current_temp_outline: "0xFFFFFF"
              current_temp_text: "0xFFFFFF"
              setpoint_bg: "0xFFA500"
              setpoint_outline: "0xFFFFFF"
              setpoint_text: "0xFFFFFF"
              current_temp_marker: "0xFFFFFF"
              setpoint_marker: "0xFFFF00"
              badge_heating_bg: "0xFF0000"
              badge_cooling_bg: "0x0000FF"
              badge_idle_bg: "0xFFA500"
              badge_off_bg: "0x808080"
              badge_heating_text: "0xFFFFFF"
              badge_cooling_text: "0xFFFFFF"
              badge_idle_text: "0xFFFFFF"
              badge_off_text: "0xFFFFFF"
              bottom_bar_bg: "0x000080"
              bottom_bar_text: "0xFFFFFF"
```

### Example 3: Partial Customization
```yaml
shys_m5_dial:
  devices:
    climates:
      - entity: climate.office
        name: "Office"
        modes:
          temp_mode:
            min_temperature: 18
            max_temperature: 25
            rotary_step_width: 0.5

            colors:
              # Only customize a few colors
              screen_bg: "0x1a1a2e"           # Dark purple-blue
              badge_heating_bg: "0xFF1493"    # Deep pink
              badge_heating_text: "0xFFFFFF"  # White
              bottom_bar_bg: "0x16213e"       # Dark blue
              # All other colors will use defaults
```

### Example 4: Multiple Climate Entities with Different Themes
```yaml
shys_m5_dial:
  devices:
    climates:
      # Warm "Fire" theme
      - entity: climate.fireplace
        name: "Fireplace"
        modes:
          temp_mode:
            colors:
              screen_bg: "0x1a0a00"
              arc_heating_active: "0xFF4500"
              badge_heating_bg: "0xFF6347"
              # ... more warm colors

      # Cool "Arctic" theme
      - entity: climate.basement_ac
        name: "AC"
        modes:
          temp_mode:
            colors:
              screen_bg: "0x001a33"
              arc_cooling: "0x00BFFF"
              badge_cooling_bg: "0x4169E1"
              # ... more cool colors

      # Default theme
      - entity: climate.bedroom
        name: "Bedroom"
        modes:
          temp_mode:
            # No colors section = defaults
```

## Color Format

Colors must be specified as **hex strings** in the format `"0xRRGGBB"`:
- `0x` prefix (required)
- 6 hex digits for RGB values
- Quoted as a string
- Examples:
  - `"0xFF0000"` = Red (R=255, G=0, B=0)
  - `"0x00FF00"` = Green (R=0, G=255, B=0)
  - `"0x0000FF"` = Blue (R=0, G=0, B=255)
  - `"0xFFFFFF"` = White
  - `"0x000000"` = Black
  - `"0x808080"` = Grey

## How It Works

1. **Python Schema Validation** (`__init__.py`):
   - Validates color parameters in YAML
   - Provides default values for any unspecified colors
   - Passes configuration as JSON to C++ code

2. **Color Parsing** (`ha_device_mode_climate_temperature.h`):
   - Converts hex strings to RGB565 format (used by M5Dial display)
   - Stores colors as member variables
   - Logs color parsing for debugging

3. **Display Rendering** (`showClimateMenu()`):
   - Uses configured colors instead of hardcoded constants
   - Draws all UI elements with customized colors

## Testing Your Configuration

1. **Validate Configuration**:
   ```bash
   docker exec esphome esphome config /config/your-config.yaml
   ```

2. **Check Logs During Compilation**:
   Look for messages like:
   ```
   [CLIMATE_COLOR] Parsed color '0xFF0000' -> RGB888(r=255,g=0,b=0) -> RGB565(0xF800)
   [CLIMATE_COLOR] Climate colors configured for entity climate.fireplace
   ```

3. **Compile and Upload**:
   ```bash
   docker exec esphome esphome run /config/your-config.yaml
   ```

## Troubleshooting

### Colors not changing?
- Ensure the `colors:` section is **inside** `temp_mode:`
- Verify hex format: `"0xRRGGBB"` (with quotes)
- Check ESPHome logs for parsing errors

### Configuration validation errors?
- Ensure all color values are strings (quoted)
- Check YAML indentation (colors must be under temp_mode)
- Verify you're using the latest version of the component

### Display looks wrong?
- RGB565 conversion may slightly alter colors
- Try different color values to achieve desired appearance
- Check device logs (DEBUG level) for color parsing details

## Future Enhancements

Potential future additions (not yet implemented):
- **Color presets**: Named themes like "fire", "ocean", "forest"
- **Preset + override**: Start with a preset and override specific colors
- **Light/dark auto-switching**: Different colors based on time of day
- **Temperature-dependent colors**: Arc colors change based on temperature

## Contributing

Found a bug or have a feature request? Please open an issue on GitHub!

## See Also

- `CLIMATE_COLOR_EXAMPLE.yaml` - More color theme examples
- `CLIMATE_MIGRATION_NOTES.md` - Climate component architecture
- `plans.md` - Component development roadmap
