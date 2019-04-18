#ifndef _ASW_H_
#define _ASW_H_

#include "RTE.h"
#include "asw_com.h"

enum _State {S1, S2, S3};
enum _Direction {LEFT, CENTER, RIGHT};

struct _car {
    T_F16 f16Speed;
    T_U8 u8IdRoad;
    enum _Direction direction;
    enum _State state;
    BOOL bWait;
};

void vInit_Everything();
void vFollower();
void vTestDistance();

void vDoS1Action();
void vDoS2Action();
void vDoS3Action();
void vFSM();

void vWait();

#endif