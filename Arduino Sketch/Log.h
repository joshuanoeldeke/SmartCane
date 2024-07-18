#ifndef BF5DB609_C4D2_4298_8733_DF5B7F256F06
#define BF5DB609_C4D2_4298_8733_DF5B7F256F06

#ifdef DEBUG
#define LOG(str) Serial.print(str);
#define LOGLN(str) Serial.println(str);
#else
#define LOG(str)
#define LOGLN(str)
#endif

#endif /* BF5DB609_C4D2_4298_8733_DF5B7F256F06 */