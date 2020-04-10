#pragma once

#include <string>

namespace elona
{
namespace snail
{
namespace audio
{

void set_position(int, short, unsigned char);

int DSINIT();
void DSLOADFNAME(const std::string& filepath, int id);
void DSPLAY(int, bool);
void DSSTOP(int);
void DSSETVOLUME(int, int);
bool CHECKPLAY(int channel);

int DMINIT();
void DMLOADFNAME(const std::string& filepath, int);
void DMPLAY(int, int);
void DMSTOP();



namespace music
{

// volume: 0 - 128
int get_volume();
void set_volume(int volume);

} // namespace music

} // namespace audio
} // namespace snail
} // namespace elona
