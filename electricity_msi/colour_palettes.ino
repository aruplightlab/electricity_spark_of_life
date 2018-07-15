DEFINE_GRADIENT_PALETTE( aiml_gp ) {
  0,     0,  0,  0,   //black
  50,     0,  0,  255,   //blue
  100,     0,  255,  255,   //cyan
  255,     255,  255,  255,   //white
};

DEFINE_GRADIENT_PALETTE( pm_gp ) {
  0,     0,  255,  0,   //green
  100,     255,  255,  0,   //yellow
  200,     255,  0,  0,   //red
  255,     255,  0,  128,   //pink
};

DEFINE_GRADIENT_PALETTE( blue_gp ) {
  0,     0,  0,  0,   //black
255,     0,  0,  255 }; //blue

DEFINE_GRADIENT_PALETTE( green_gp ) {
  0,     0,  0,  0,   //black
255,     0,  255,  0 }; //green

DEFINE_GRADIENT_PALETTE( yellow_gp ) {
  0,     0,  0,  0,   //black
255,     255,  255,  0 }; //yellow

DEFINE_GRADIENT_PALETTE( orange_gp ) {
  0,     0,  0,  0,   //black
255,     255, 128,  0 }; //orange

DEFINE_GRADIENT_PALETTE( red_gp ) {
  0,     0,  0,  0,   //black
255,     255,  0,  0 }; //red

DEFINE_GRADIENT_PALETTE( pink_gp ) {
  0,     0,  0,  0,   //black
255,     255,  0,  128 }; //pink

DEFINE_GRADIENT_PALETTE( white_gp ) {
  0,     0,  0,  0,   //black
255,     255,  255,  255 }; //white

DEFINE_GRADIENT_PALETTE( black_gp ) {
  0,     0,  0,  0,   //black
255,     0,  0,  0 }; //black

void setColor(int color) {
  int i;
  Serial.println(color);
  CRGB c;
  switch (color) {
    case 1:
      c = CRGB::White;
      palette = white_gp;
      break;
    case 2:
      c = CRGB::Red;
      palette = red_gp;
      break;
    case 3:
      c = CRGB::Green;
      palette = green_gp;
      break;
    case 4:
      c = CRGB::Blue;
      palette = blue_gp;
      break;
    case 5:
      c = CRGB::Yellow;
      palette = yellow_gp;
      break;
    case 6:
      c = CRGB::Orange;
      palette = orange_gp;
      break;
    case 7:
      c = CRGB::Pink;
      palette = pink_gp;
      break;
    case 100:
      c = CRGB::White;
      palette = aiml_gp;
      break;
    case 0:
      c = CRGB::Black;
      palette = black_gp;
      break;
  }
    delay(DELAY);
}
