#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <unordered_map>

#include "bullet_common.hpp"

#define DIALOG_SKIN

typedef bool (*DialogBoxDrawerFunc)(const char* dialog, std::vector<std::string> keywords, Texture canvas, float timeSince);

struct DialogLine {
    std::string text;
    std::vector<std::string> keywords;
};

struct DialogSequence {
    std::string name;
    std::vector<DialogLine> lines;

    DialogLine getDialogLine(size_t index) const
    {
        return lines.at(index);
    }
};

class Dialog : public Service<Dialog> {
   public:
    static void StartSequence(std::string dialog);

    void RegisterSequence(const char* name, const char* lines);
    bool UpdateAndRenderBoxes();

   private:
    Dialog() noexcept;
    std::unordered_map<std::string, DialogSequence> sequences;

    std::string curSequence;
    bool isSpeaking;
    double startTime;
    size_t lineIndex;

    static DIALOG_SKIN bool DrawDefaultDialogBox(std::string& dialog, std::vector<std::string> keywords, Texture canvas, float timeSince);

    static const DialogSequence& GetCurrentSequence();
    static const DialogSequence& GetSequence(const std::string& name);

    // utilities
    static void FormatLine(std::string& line);
    static std::vector<std::string> ExtractKeywords(std::string& line);
    static std::vector<std::string> ExtractLines(std::string& lines);
};
