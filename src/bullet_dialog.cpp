#include <cstring>
#include <raylib.h>

#include "bullet_assets.hpp"
#include "bullet_dialog.hpp"

using namespace std;

Dialog::Dialog() noexcept : Service("Dialog")
{
    this->sequences = {};
    this->isSpeaking = false;
    this->startTime = 0;
    this->lineIndex = 0;

    Dialog::RegisterSequence("TUTORIAL_START",
                             "{placeholder|excited}Greetings traveler, are you up for the challenge to reach the mountain top?\\\
                            {placeholder|happy}It won't be an easy journey but I'll try to help you on your way!\\\
                            {placeholder|neutral}See your spaceship in the middle of the screen? That's you!\\\
                            {placeholder|happy}Move it around with the arrow keys or WASD (will be changed in the future)\\\
                            {placeholder|shout}Press Z to fire your ammunition! Without it, you'll be fried meat before you even realize...");
}

DIALOG_SKIN bool Dialog::DrawDefaultDialogBox(std::string& dialog, vector<string> keywords, Texture canvas, float timeSince)
{
    if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        return true;
    }

    const int margin = 10;
    const int padding = 10;
    const int height = 80;

    const int topY = HEIGHT - height - margin;

    // first keyword is the avatar collection name
    // second keyword is the emotion used
    // render avatar texture
    if (keywords.size() > 0) {
        std::string avatar = keywords[0];
        std::string emotion = keywords.size() > 1 ? keywords[1] : "neutral";

        const int size = height;
        const char* avatarPath = TextFormat("spr_dialog_%s_%s", avatar.c_str(), emotion.c_str());

        Texture avatarTexture = Assets::RequestTexture(avatarPath);  // TODO: Add placeholder texture
        Vector2 avatarPos = { margin, topY };
        DrawTextureEx(avatarTexture, avatarPos, 0, size, WHITE);

        DrawRectangle(margin + height + padding, topY, WIDTH - 2 * margin * 2 - height, height, BLACK);
        DrawText(dialog.c_str(), margin + height + padding * 2, topY + padding, 12, WHITE);

        std::string info = emotion + "\n" + avatarPath;
        DrawText(info.c_str(), 150, 300, 12, GREEN);
    } else {
        DrawRectangle(margin, topY, WIDTH - 2 * margin, height, BLACK);
        DrawText(dialog.c_str(), margin + padding, topY + padding, 12, WHITE);
    }
    return false;
}

void Dialog::StartSequence(string dialog)
{
    DialogSequence sequence;
    try {
        sequence = this->GetSequence(dialog);
    } catch (std::out_of_range e) {
        TraceLog(LOG_WARNING, "No registered dialog sequences named %s !", dialog.c_str());
        return;
    }

    if (!curSequence.empty() && Dialog.curSequence == dialog) {
        TraceLog(LOG_WARNING, "Already speaking, can't start dialog %s", dialog.c_str());
        return;
    }

    // start speaking
    Dialog.curSequence = dialog;
    Dialog.isSpeaking = true;
    Dialog.startTime = GetTime();
    TraceLog(LOG_DEBUG, "> Started dialog sequence %s", dialog);
}

func bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc)
{
    if (!Dialog.isSpeaking) {
        return false;
    }

    DialogSequence curSequence;
    try {
        curSequence = Dialog.getCurrentSequence();
    } catch (std::out_of_range e) {
        TraceLog(LOG_WARNING, "Invalid dialog state, reverting...");
        Dialog.isSpeaking = false;
        return false;
    }

    DialogLine line;
    try {
        line = curSequence.getDialogLine(Dialog.lineIndex);
    } catch (std::out_of_range e) {
        TraceLog(LOG_WARNING, "No dialog line with index %zu", Dialog.lineIndex);
        return false;
    }

    // advance to next line if confirmed
    float elapsed = (float)(GetTime() - Dialog.startTime);
    if ((*drawingFunc)(line.text.c_str(), line.keywords, canvas, elapsed)) {
        Dialog.lineIndex++;
        if (Dialog.lineIndex >= curSequence.lines.size()) {
            Dialog.isSpeaking = false;
            TraceLog(LOG_DEBUG, "< Ended dialog sequence %s", Dialog.curSequence.c_str());
        } else {
            TraceLog(LOG_DEBUG, ">>> %s", Dialog.curSequence.c_str());
        }
    }
    return true;
}

bool UpdateAndRenderDialogBoxes(Texture canvas)
{
    return UpdateAndRenderCustomDialogBoxes(canvas, DrawDefaultDialogBox);
}

void RegisterDialogSequence(const char* name, const char* lines)
{
    DialogSequence seq = DialogSequence();
    seq.name = name;

    // extract lines from compact 'lines' parameter
    StringArray sa = ExtractDialogLines(lines);
    for (size_t i = 0; i < sa.count; i++) {
        const char* text = sa.strings[i];
        StringArray keywordsC = ExtractDialogKeywords(text);

        FormatDialogLine(text);

        DialogLine line;
        line.text = text;

        // HACK: bridge
        line.keywords = StringArrayToVector(keywordsC);
        seq.lines.push_back(line);
    }
    DisposeStringArray(&sa);

    Dialog.sequences.insert({ name, seq });

    SDL_LogDebug(0, "Registered dialog sequence %s with %zu lines.", name, seq.lines.size());
}

void FormatDialogLine(const char* line)
{
    char* clone = (char*)CloneText(line);  // TODO: lazy

    usize j = 0;
    bool penDown = true;
    for (usize i = 0; i < strlen(clone); i++) {
        char token = clone[i];
        if (token == '{') {
            penDown = false;
        } else if (token == '}') {
            penDown = true;
            continue;
        }
        if (penDown) {
            ((char*)line)[j++] = token;
        }
    }

    // null terminate so we don't have any garbage
    ((char*)line)[j] = '\0';

    free(clone);
}

std::vector<std::string> ExtractDialogKeywords(const char* line)
{
    usize keywordStart = 0;
    for (usize i = 0; i < strlen(line); i++) {
        char token = line[i];
        if (token == '{') {
            if (keywordStart > 0) {
                TraceLog(LOG_ERROR, "Duplicate start brace for dialog line %s", line);
            }
            keywordStart = i + 1;
        } else if (token == '}' || token == '|') {
            if (keywordStart == 0) {
                TraceLog(LOG_ERROR, "No start brace found for dialog line %s", line);
            }

            // copy the keyword over and place it into the stringarray
            usize len = i - keywordStart;
            char* slice = malloc(len + 1);
            memcpy(slice, &line[keywordStart], len);
            slice[len] = '\0';
            StoreString(&result, slice);
            free(slice);

            if (token == '}')  // reached the end
            {
                break;
            } else {
                keywordStart = i + 1;
            }
        }
    }
    return result;
}

vector<string> ExtractDialogLines(string& lines)
{
    char* buffer = CloneText(lines);

    StringArray result = InitStringArray();
    const char* SEP = "\\";
    char* line = strtok(buffer, SEP);
    while (line != NULL) {
        StripText(line);
        StoreString(&result, line);
        line = strtok(NULL, SEP);
    }

    free(buffer);
    return result;
}

const DialogSequence& Dialog::GetCurrentSequence()
{
    auto dialog = instance.lock();
    return dialog->GetSequence(dialog->curSequence);
}

const DialogSequence& Dialog::GetSequence(const std::string& name)
{
    auto dialog = instance.lock();
    return dialog->sequences.at(name);
}
