/****************************************************************************
 * Contest 2026 team 039 - VelaBridge device state machine skeleton
 ****************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "velabridge_watch_ui.h"

enum velabridge_state
{
  VELABRIDGE_STATE_NORMAL = 0,
  VELABRIDGE_STATE_CAPTION_MODE,
  VELABRIDGE_STATE_BLIND_MODE,
  VELABRIDGE_STATE_OCR_MODE,
  VELABRIDGE_STATE_HELP_MODE,
  VELABRIDGE_STATE_DEMO_MODE,
};

enum velabridge_event
{
  EVT_BOOT = 0,
  EVT_KEY_SHORT,
  EVT_KEY_LONG,
  EVT_JSON_CAPTION,
  EVT_JSON_DANGER,
  EVT_JSON_HELP,
  EVT_TIMEOUT,
  EVT_ERROR,
};

enum velabridge_vibration_pattern
{
  VELABRIDGE_VIBRATE_NONE = 0,
  VELABRIDGE_VIBRATE_SHORT,
  VELABRIDGE_VIBRATE_LONG,
  VELABRIDGE_VIBRATE_URGENT,
  VELABRIDGE_VIBRATE_DOUBLE,
};

static enum velabridge_state g_velabridge_state =
  VELABRIDGE_STATE_NORMAL;

static bool velabridge_is_ascii_log_text(const char *text)
{
  const unsigned char *cursor = (const unsigned char *)text;

  while (cursor != NULL && *cursor != '\0')
    {
      if (*cursor >= 0x80)
        {
          return false;
        }

      cursor++;
    }

  return true;
}

static const char *velabridge_log_text(const char *text)
{
  if (text == NULL)
    {
      return "";
    }

  return velabridge_is_ascii_log_text(text) ? text : "non_ascii_text";
}

static void velabridge_log_ai_command(const char *command)
{
  printf("[velabridge][cmd] %s\n", command ? command : "unknown");
}

static const char *velabridge_state_name(enum velabridge_state state)
{
  switch (state)
    {
      case VELABRIDGE_STATE_NORMAL:
        return "NORMAL";
      case VELABRIDGE_STATE_CAPTION_MODE:
        return "CAPTION_MODE";
      case VELABRIDGE_STATE_BLIND_MODE:
        return "BLIND_MODE";
      case VELABRIDGE_STATE_OCR_MODE:
        return "OCR_MODE";
      case VELABRIDGE_STATE_HELP_MODE:
        return "HELP_MODE";
      case VELABRIDGE_STATE_DEMO_MODE:
        return "DEMO_MODE";
      default:
        return "UNKNOWN";
    }
}

static const char *velabridge_event_name(enum velabridge_event event)
{
  switch (event)
    {
      case EVT_BOOT:
        return "EVT_BOOT";
      case EVT_KEY_SHORT:
        return "EVT_KEY_SHORT";
      case EVT_KEY_LONG:
        return "EVT_KEY_LONG";
      case EVT_JSON_CAPTION:
        return "EVT_JSON_CAPTION";
      case EVT_JSON_DANGER:
        return "EVT_JSON_DANGER";
      case EVT_JSON_HELP:
        return "EVT_JSON_HELP";
      case EVT_TIMEOUT:
        return "EVT_TIMEOUT";
      case EVT_ERROR:
        return "EVT_ERROR";
      default:
        return "EVT_UNKNOWN";
    }
}

static const char *velabridge_vibration_name(
  enum velabridge_vibration_pattern pattern)
{
  switch (pattern)
    {
      case VELABRIDGE_VIBRATE_NONE:
        return "none";
      case VELABRIDGE_VIBRATE_SHORT:
        return "short";
      case VELABRIDGE_VIBRATE_LONG:
        return "long";
      case VELABRIDGE_VIBRATE_URGENT:
        return "urgent";
      case VELABRIDGE_VIBRATE_DOUBLE:
        return "double";
      default:
        return "unknown";
    }
}

void velabridge_display_text(const char *text)
{
  printf("[velabridge][display] %s\n", velabridge_log_text(text));
}

void velabridge_play_prompt(const char *prompt_id)
{
  printf("[velabridge][prompt] %s\n", velabridge_log_text(prompt_id));
}

void velabridge_vibrate(enum velabridge_vibration_pattern pattern)
{
  printf("[velabridge][vibration] %s\n",
         velabridge_vibration_name(pattern));
}

void velabridge_log_event(enum velabridge_event event, const char *detail)
{
  printf("[velabridge][event] state=%s event=%s detail=%s\n",
         velabridge_state_name(g_velabridge_state),
         velabridge_event_name(event),
         velabridge_log_text(detail));
}

static void velabridge_set_state(enum velabridge_state next_state,
                                 const char *reason)
{
  enum velabridge_state previous_state = g_velabridge_state;

  if (previous_state != next_state)
    {
      printf("[velabridge][state] %s -> %s reason=%s\n",
             velabridge_state_name(previous_state),
             velabridge_state_name(next_state),
             reason ? reason : "");
    }

  g_velabridge_state = next_state;
}

static void velabridge_dispatch_event(enum velabridge_event event,
                                      const char *payload)
{
  velabridge_log_event(event, payload);

  switch (event)
    {
      case EVT_BOOT:
        velabridge_set_state(VELABRIDGE_STATE_NORMAL, "boot");
        velabridge_display_text("VelaBridge ready");
        velabridge_vibrate(VELABRIDGE_VIBRATE_NONE);
        break;

      case EVT_KEY_SHORT:
        velabridge_set_state(VELABRIDGE_STATE_CAPTION_MODE, "key_short");
        velabridge_display_text("caption mode");
        velabridge_vibrate(VELABRIDGE_VIBRATE_SHORT);
        break;

      case EVT_KEY_LONG:
        velabridge_set_state(VELABRIDGE_STATE_HELP_MODE, "key_long");
        velabridge_display_text("help mode");
        velabridge_play_prompt("help_triggered");
        velabridge_vibrate(VELABRIDGE_VIBRATE_LONG);
        break;

      case EVT_JSON_CAPTION:
        velabridge_set_state(VELABRIDGE_STATE_CAPTION_MODE,
                             "json_caption");
        velabridge_display_text(payload ? payload : "caption");
        break;

      case EVT_JSON_DANGER:
        velabridge_set_state(VELABRIDGE_STATE_CAPTION_MODE,
                             "json_danger");
        velabridge_display_text("danger alert");
        velabridge_play_prompt("danger_alert");
        velabridge_vibrate(VELABRIDGE_VIBRATE_URGENT);
        break;

      case EVT_JSON_HELP:
        velabridge_set_state(VELABRIDGE_STATE_HELP_MODE, "json_help");
        velabridge_display_text("help requested");
        velabridge_play_prompt("help_requested");
        velabridge_vibrate(VELABRIDGE_VIBRATE_LONG);
        break;

      case EVT_TIMEOUT:
        velabridge_set_state(VELABRIDGE_STATE_NORMAL, "timeout");
        velabridge_display_text("waiting for JSON messages");
        velabridge_vibrate(VELABRIDGE_VIBRATE_NONE);
        break;

      case EVT_ERROR:
        velabridge_display_text("protocol error");
        velabridge_vibrate(VELABRIDGE_VIBRATE_DOUBLE);
        break;

      default:
        velabridge_display_text("unknown event");
        velabridge_vibrate(VELABRIDGE_VIBRATE_DOUBLE);
        break;
    }
}

static bool velabridge_line_contains(const char *line, const char *needle)
{
  return line != NULL && needle != NULL && strstr(line, needle) != NULL;
}

static bool velabridge_handle_open_command(const char *line)
{
  const char *target;
  char target_id[16];
  size_t i = 0;

  if (line == NULL || strncmp(line, "VB_OPEN", 7) != 0)
    {
      return false;
    }

  target = line + 7;
  while (*target == ' ')
    {
      target++;
    }

  if (*target == '\0')
    {
      return true;
    }

  while (target[i] != '\0' && target[i] != '\r' && target[i] != '\n' &&
         target[i] != ' ' && i < sizeof(target_id) - 1)
    {
      target_id[i] = target[i];
      i++;
    }

  target_id[i] = '\0';
  printf("[velabridge][cmd] VB_OPEN %s\n", target_id);
  (void)velabridge_watch_ui_open(target_id);
  return true;
}

static bool velabridge_parse_command_value(const char *line,
                                           const char *command,
                                           char *value,
                                           size_t value_size)
{
  const char *cursor;
  size_t command_len;
  size_t value_len;

  if (line == NULL || command == NULL || value == NULL ||
      value_size == 0)
    {
      return false;
    }

  command_len = strlen(command);
  if (strncmp(line, command, command_len) != 0)
    {
      return false;
    }

  cursor = line + command_len;
  if (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' &&
      *cursor != '\r' && *cursor != '\n')
    {
      return false;
    }

  while (*cursor == ' ' || *cursor == '\t')
    {
      cursor++;
    }

  value_len = strcspn(cursor, "\r\n");
  if (value_len >= value_size)
    {
      value_len = value_size - 1;
    }

  memcpy(value, cursor, value_len);
  value[value_len] = '\0';
  return true;
}

static bool velabridge_handle_ai_command(const char *line)
{
  char value[128];

  if (velabridge_parse_command_value(line, "VB_CAPTION", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_CAPTION");
      (void)velabridge_watch_ui_set_caption(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_OCR", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_OCR");
      (void)velabridge_watch_ui_set_ocr(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_ALERT", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_ALERT");
      (void)velabridge_watch_ui_set_alert(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_SCENE", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_SCENE");
      (void)velabridge_watch_ui_set_scene(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_RISK", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_RISK");
      (void)velabridge_watch_ui_set_risk(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_ADVICE", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_ADVICE");
      (void)velabridge_watch_ui_set_advice(value);
      return true;
    }

  if (velabridge_parse_command_value(line, "VB_REPLY", value,
                                     sizeof(value)))
    {
      velabridge_log_ai_command("VB_REPLY");
      (void)velabridge_watch_ui_set_reply(value);
      return true;
    }

  return false;
}

void velabridge_handle_json_line(const char *line)
{
  if (velabridge_handle_open_command(line))
    {
      return;
    }

  if (velabridge_handle_ai_command(line))
    {
      return;
    }

  printf("[velabridge][json] %s\n", velabridge_log_text(line));

  if (line == NULL || line[0] == '\0')
    {
      velabridge_dispatch_event(EVT_ERROR, "empty_json_line");
    }
  else if (velabridge_line_contains(line, "danger_alert") ||
           velabridge_line_contains(line, "danger"))
    {
      velabridge_dispatch_event(EVT_JSON_DANGER, line);
    }
  else if (velabridge_line_contains(line, "help"))
    {
      velabridge_dispatch_event(EVT_JSON_HELP, line);
    }
  else if (velabridge_line_contains(line, "caption"))
    {
      velabridge_dispatch_event(EVT_JSON_CAPTION, line);
    }
  else
    {
      velabridge_dispatch_event(EVT_ERROR, "unsupported_json_type");
    }
}

int main(int argc, char *argv[])
{
  int ret;

  (void)argc;
  (void)argv;

  ret = velabridge_watch_ui_start();
  if (ret == 0)
    {
      return 0;
    }

  printf("[velabridge][watch_ui] fallback to serial demo ret=%d\n", ret);
  printf("\n");
  printf("VelaBridge Watch started\n");
  printf("Board: SF32LB52 DevKit ULP\n");
  printf("Mode: normal\n");
  printf("Blind mode: available\n");
  printf("AI bridge: mock ready\n");
  printf("\n");
  printf("[caption] Teacher: please go to the academic service desk.\n");
  printf("[ocr] Cafeteria Window 1: today's meal is 15 yuan.\n");
  printf("[danger_alert] Step detected ahead, please be careful.\n");
  printf("[quick_reply] Please speak slower, I am reading captions.\n");
  printf("\n");
  printf("VelaBridge Watch demo finished\n");
  fflush(stdout);

  return 0;
}
