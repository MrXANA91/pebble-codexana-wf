module.exports = [
  {
    "type": "heading",
    "defaultValue": "Code XANA Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "defaultValue": "0x000000",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "ForegroundColor",
        "defaultValue": "0xFFFFFF",
        "label": "Foreground Color"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Logo behavior"
      },
      {
        "type": "toggle",
        "messageKey": "ShowLogo",
        "label": "Show Logo",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "InvertLogoStateOnDisconnect",
        "label": "Invert Logo state when phone connection is lost",
        "defaultValue": true,
        "description": "This will invert the default state of the logo when losing phone connection. For example, if 'Show Logo' is disable and this is enabled, logo will appear when connection is lost. By default, logo will disappear when connection is lost."
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "More options"
      },
      {
        "type": "toggle",
        "messageKey": "VibrateOnDisconnect",
        "label": "Vibrate on disconnect",
        "defaultValue": false
      },
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];