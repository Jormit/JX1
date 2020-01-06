#include "osc.h"
#include <math.h>

/** Converts keycode into a note value. **/
char Key2Note(KEY_EVENT_RECORD ker) {
    char note = 0;
    switch(ker.wVirtualKeyCode){
    case 'A':
        note = 1;
        break;
    case 'W':
        note = 2;
        break;
    case 'S':
        note = 3;
        break;
    case 'E':
        note = 4;
        break;
    case 'D':
        note = 5;
        break;
    case 'F':
        note = 6;
        break;
    case 'T':
        note = 7;
        break;
    case 'G':
        note = 8;
        break;
    case 'Y':
        note = 9;
        break;
    case 'H':
        note = 10;
        break;
    case 'U':
        note = 11;
        break;
    case 'J':
        note = 12;
        break;
    }

    return note;
}

/** Creates a clean notes struct **/
VOID initialize_keys(notes_pressed *notes){
    notes->note1.code = 0;
    notes->note1.stage = 0.0;
    notes->note1.time = 0.0;
}

/** Given a key event this will set the current notes accordingly. **/
VOID KeyEventProc(KEY_EVENT_RECORD ker, notes_pressed *notes) {
    char note = Key2Note(ker);

    if (ker.bKeyDown) { // Key press.
        if (notes->note1.code == note || notes->note2.code == note ||
        notes->note3.code == note || notes->note4.code == note ) {return;}

        if (note && note != notes->note1.code) {
            float freq = 220.0 * pow(2.0, ((float)note + 2.0)/ 12.0);
            notes->note1.code = note;
            notes->note1.freq = freq;
            notes->note1.time = 0.0;
            notes->note1.stage = 0;
        }
    } else { // Key release.
         if (notes->note1.code == note) {
            notes->note1.code = 0;
            notes->note1.stage = 3;
        }
    }
}
