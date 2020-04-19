struct bcrypt{
public:
  static uint8_t cost = 10;
  static uint8_t[] bcrypt(uint8_t pass[72]);

private:
  static EksBlowFish(uint8_t salt[16], uint8_t pass[72]);
   

};
