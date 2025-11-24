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
        "defaultValue": "0xFFFFFF",
        "label": "Background Color",
        "capabilities": [ "COLOR" ]
      },
      {
        "type": "color",
        "messageKey": "TextColor",
        "defaultValue": "0x000000",
        "label": "Text Color",
        "capabilities": [ "COLOR" ]
      },
      {
        "type": "color",
        "messageKey": "EyeColor",
        "defaultValue": "0xAA0000",
        "label": "Eye Color",
        "capabilities": [ "COLOR" ]
      },
      {
        "type": "color",
        "messageKey": "EyeGrayedColor",
        "defaultValue": "0xAAAAAA",
        "label": "Eye Gray Color",
        "capabilities": [ "COLOR" ]
      },
      {
        "type": "toggle",
        "messageKey": "DarkMode",
        "defaultValue": false,
        "label": "Dark Mode",
        "capabilities": [ "BW" ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "XANA Eye"
      },
      {
        "type": "toggle",
        "messageKey": "VoidOnDisconnect",
        "label": "Central circles disappear on lost connection",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "GrayOnDisconnect",
        "label": "Gray out on lost connection",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Vibes"
      },
      {
        "type": "toggle",
        "messageKey": "VibrateOnDisconnect",
        "label": "Vibrate on disconnect",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];