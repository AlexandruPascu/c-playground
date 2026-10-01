#ifndef PLAYGROUND_DEMOS_H
#define PLAYGROUND_DEMOS_H
enum PgDemo { PG_HELLO, PG_ART, PG_MOTION, PG_CLOSE, PG_EVENTS, PG_SQUARE, PG_COIN, PG_CHARACTER };
int pg_run_demo(int argc, char **argv, enum PgDemo demo);
#endif
