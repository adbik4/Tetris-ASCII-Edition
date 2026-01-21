#include "SoundFX.h"
#include "CursesWinCompat.h"

// Plays a happy successful sound
// Args: none
// Returns: void
void successSound() {
	Beep(1976, 100);
	Beep(2637, 60);
	flushinp();		// discard any user input collected during the SFX
}

// Plays a sad failure sound
// Args: none
// Returns: void
void failureSound() {
	Beep(587, 300);
	Beep(554, 300);
	Beep(523, 300);
	Beep(494, 300);
	flushinp();		// discard any user input collected during the SFX
}