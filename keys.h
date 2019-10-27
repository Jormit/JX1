#ifndef KEYS_H_INCLUDED
#define KEYS_H_INCLUDED
#include <windows.h>

typedef struct {
    char code;
    float freq;
} note;

// All the notes played at a particular instant. Max 4.
typedef struct {
    note note1;
    note note2;
    note note3;
    note note4;
} notes_pressed;

char Key2Note(KEY_EVENT_RECORD ker);
VOID KeyEventProc(KEY_EVENT_RECORD ker, notes_pressed *notes);
VOID initialize_keys(notes_pressed *notes);

#endif // KEYS_H_INCLUDED
