/* Mariana's Space Adventure
 * Copyright (C) Dave Nicholas 2010 <dave@davenicholas.me.uk>
 * 
 * Mariana's Space Adventure is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Mariana's Space Adventure is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 

#include "sound.h"	

Mix_Chunk* sound;
Mix_Music* music;

void InitMusic() {
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16;
	int audio_channels = 2;
	int audio_buffers = 4096;
	Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);	
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
}

void PlayMenuBackgroundMusic() {
	music = Mix_LoadMUS(MENU_MUSIC);
	Mix_PlayMusic(music, 0);
	Mix_HookMusicFinished(MenuBackgroundMusicFinishedCallback);
}

void MenuBackgroundMusicFinishedCallback() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
	PlayMenuBackgroundMusic();
}

void PlayBackgroundMusic() {
	music = Mix_LoadMUS(GAME_MUSIC);
	Mix_PlayMusic(music, 0);
	Mix_HookMusicFinished(BackgroundMusicFinishedCallback);
}

void BackgroundMusicFinishedCallback() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
	PlayBackgroundMusic();
}

void CloseAudio() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_HaltChannel(-1);
	Mix_FreeChunk(sound);
	Mix_CloseAudio();
}
