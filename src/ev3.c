#include <mruby.h>
#include <mruby/data.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


struct ev3_state {
  int lms_ui;
  unsigned char *keypad;
};

static void ev3_free(mrb_state *mrb, void *ptr) {
  struct ev3_state *state = (struct ev3_state *)(ptr);

  if (state->keypad != MAP_FAILED) {
    munmap(state->keypad, 6);
  }
  if(state->lms_ui > 0) {
    close(state->lms_ui);
  }
  mrb_free(mrb, state);
}

static struct mrb_data_type ev3_type = { "EV3", ev3_free };

static mrb_value
mrb_ev3_initialize(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = mrb_malloc(mrb, sizeof(struct ev3_state));

  DATA_TYPE(self) = &ev3_type;
  DATA_PTR(self) = state;

  state->lms_ui = open("/dev/lms_ui", O_RDWR|O_SYNC);

  state->keypad = MAP_FAILED;
  if(state->lms_ui > 0) {
    state->keypad = mmap(NULL, 6, PROT_READ|PROT_WRITE, MAP_SHARED, state->lms_ui, 0);
  }
  return mrb_nil_value();
}

static mrb_value
mrb_ev3_keypad(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  mrb_int key;

  mrb_get_args(mrb, "i", &key);
  if(key >= 0 && key <= 5) {
    return *(state->keypad + key) == 1 ? mrb_true_value() : mrb_false_value();
  }

  return mrb_false_value();
}

static mrb_value
mrb_ev3_set_led(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  mrb_int key;

  mrb_get_args(mrb, "i", &key);
  if(key >= 0 && key <= 5) {
    char buf[2] = "0";
    buf[0] = '0'+key;
    write(state->lms_ui, buf, 2);
  }
  return mrb_fixnum_value(key);
}

void
mrb_mruby_mindstorms_ev3_gem_init(mrb_state* mrb) {
  struct RClass *ev3_class = mrb_define_class(mrb, "EV3", mrb->object_class);
  mrb_define_method(mrb, ev3_class, "initialize", mrb_ev3_initialize, ARGS_NONE());
  mrb_define_method(mrb, ev3_class, "keypad?", mrb_ev3_keypad, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ev3_class, "led=", mrb_ev3_set_led, MRB_ARGS_REQ(1));
}

void
mrb_mruby_mindstorms_ev3_gem_final(mrb_state* mrb) {
  /* free someone */
}
