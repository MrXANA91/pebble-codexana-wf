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
        "type": "color",
        "messageKey": "BatteryLevelColor",
        "defaultValue": "0x550000",
        "label": "Battery Level Color",
        "capabilities": [ "COLOR" ]
      },
      {
        "type": "color",
        "messageKey": "StepCounterColor",
        "defaultValue": "0x000055",
        "label": "Steps Counter Color",
        "capabilities": [ "COLOR", "HEALTH" ]
      },
      {
        "type": "toggle",
        "messageKey": "DarkMode",
        "defaultValue": false,
        "label": "Dark Mode",
        "capabilities": [ "BW" ]
      },
      {
        "type": "toggle",
        "messageKey": "GrayBatteryBar",
        "defaultValue": true,
        "label": "Gray Battery Bar Level",
        "capabilities": [ "BW" ],
        "description": "Enable this to set the battery bar color to gray. If disabled, color will be same as text"
      },
      {
        "type": "toggle",
        "messageKey": "GrayStepCounter",
        "defaultValue": true,
        "label": "Gray Steps Bar Counter",
        "capabilities": [ "BW", "HEALTH" ],
        "description": "Enable this to set the steps bar color to gray. If disabled, color will be same as text"
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
        "defaultValue": "Battery"
      },
      {
        "type": "toggle",
        "messageKey": "DisplayBattery",
        "label": "Display Battery",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "section",
    "capabilities": [ "HEALTH" ],
    "items": [
      {
        "type": "heading",
        "defaultValue": "Steps"
      },
      {
        "type": "toggle",
        "messageKey": "DisplaySteps",
        "label": "Display Steps",
        "defaultValue": true
      },
      {
        "type": "slider",
        "messageKey": "StepsBarMax",
        "label": "Steps Bar Max",
        "defaultValue": 5000,
        "step": 100,
        "min": 100,
        "max": 50000
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