#ifndef _ASW_H_
#define _ASW_H_

#include <stdlib.h>
#include "RTE.h"

enum _State {S1, S2, S3};
enum _Direction {LEFT, CENTER, RIGHT};

void vInit_Everything();
void vFollower();
void vTestDistance();

void vDoS1Action();
void vDoS2Action();
void vDoS3Action();
void vFSM();

#endif