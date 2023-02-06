#include "header.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(COOL_5, OUTPUT); // initialize digital LED_15 as an output.
  digitalWrite(COOL_5, HIGH);
  pinMode(COOL_18, OUTPUT); // initialize digital LED_2 as an output.
  digitalWrite(COOL_18, HIGH);
  pinMode(COOL_19, OUTPUT); // initialize digital LED_4 as an output.
  digitalWrite(COOL_19, HIGH);

  pinMode(LED_15, OUTPUT); // initialize digital LED_15 as an output.
  digitalWrite(LED_15, HIGH);
  pinMode(LED_2, OUTPUT); // initialize digital LED_2 as an output.
  digitalWrite(LED_2, HIGH);
  pinMode(LED_4, OUTPUT); // initialize digital LED_4 as an output.
  digitalWrite(LED_4, HIGH);
  delay(10);




  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage("1729540434", "Power just turned on, please reconfig timer with /settimer, if your start hour has already passed remember to turn light on and manualy in the first day", "");
}

void loop()
{
 
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    // while /settimer not called on telegram, loop wont call ledTimer()
    if (noTimerSet == 1 ){
      ledTimer(start_time1, start_time2, "1729540434");
    }
    
    
    bot_lasttime = millis();
  }
}

void ledTimer(int startHour, int startMinute , String chatId) {
  
    time_t currentTime;
    struct tm *timeInfo;


    time(&currentTime);
    timeInfo = localtime(&currentTime);

    char hour[10];
    char minutes[10];
    strftime(hour, sizeof(hour), "%H", timeInfo);
    strftime(minutes, sizeof(minutes), "%M", timeInfo);
 

    if((atoi(hour) <= startHour) && (startHour != 0)){
      stateLock = 0;
    }else if(( atoi(hour) >= 23) && (startHour == 0) ){
      stateLock = 0;
    }
    
//  bot.sendMessage(chatId,hour , "");
//  bot.sendMessage(chatId, minutes, "");/
    
    if((stateLock == 0) && (atoi(hour) >= startHour) && (atoi(minutes) >= startMinute)) {
      // turn on LEDs
      digitalWrite(COOL_5, LOW);
      digitalWrite(COOL_18, LOW);
      digitalWrite(COOL_19, LOW);
      digitalWrite(LED_15, LOW); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_4, LOW);
      ledStatus1 = 0;
      ledStatus2 = 0;
      ledStatus3 = 0;
      stateLock = 1;
      bot.sendMessage(chatId, "all Leds ON", "");
      timer_lasttime = millis();
    }
    
    if ((stateLock == 1) && (millis() - timer_lasttime  >= timerCountMs)) {
      // turn off LEDs
      digitalWrite(COOL_5, HIGH);
      digitalWrite(COOL_18, HIGH);
      digitalWrite(COOL_19, HIGH);
      digitalWrite(LED_15, HIGH); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_4, HIGH);
      ledStatus1 = 1;
      ledStatus2 = 1;
      ledStatus3 = 1;
      stateLock = 2;
      bot.sendMessage(chatId, "all Leds off", "");
    }

}


void checkLedStatus(String chatId) {
  int ledStatuses[] = {ledStatus1, ledStatus2, ledStatus3}; // array of led status variables
  String ledNames[] = {"Led 1", "Led 2", "Led 3"}; // array of led names

  
  for (int i = 0; i < 3; i++) {
    if (ledStatuses[i] == 0) {
      String message = ledNames[i] + " is ON";
      bot.sendMessage(chatId, message);
    } else {
      String message = ledNames[i] + " is OFF";
      bot.sendMessage(chatId, message);
    }
  }
}



void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/ledon")
    {
      digitalWrite(COOL_5, LOW);
      digitalWrite(COOL_18, LOW);
      digitalWrite(COOL_19, LOW);
      digitalWrite(LED_15, LOW); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_4, LOW);
      ledStatus1 = 0;
      ledStatus2 = 0;
      ledStatus3 = 0;
      bot.sendMessage(chat_id, "all Leds ON", "");
    }

    if (text == "/ledon1")
    {
      digitalWrite(COOL_5, LOW);
      digitalWrite(LED_15, LOW); // turn the LED on (HIGH is the voltage level)
      ledStatus1 = 0;
      bot.sendMessage(chat_id, "Led 1 is ON", "");
    }

     if (text == "/ledon2")
    {
      digitalWrite(COOL_18, LOW);
      digitalWrite(LED_2, LOW); // turn the LED on (HIGH is the voltage level) 
      ledStatus2 = 0;
      bot.sendMessage(chat_id, "Led 2 is ON", "");
    }

     if (text == "/ledon3")
    {
      digitalWrite(COOL_19, LOW);
      digitalWrite(LED_4, LOW); // turn the LED on (HIGH is the voltage level)
      ledStatus3 = 0;
      bot.sendMessage(chat_id, "Led 3 is ON", "");
    }

     if (text == "/ledoff")
    {
      digitalWrite(COOL_5, HIGH);
      digitalWrite(COOL_18, HIGH);
      digitalWrite(COOL_19, HIGH);
      digitalWrite(LED_15, HIGH); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_4, HIGH);
      ledStatus1 = 1;
      ledStatus2 = 1;
      ledStatus3 = 1;
      bot.sendMessage(chat_id, "all Leds off", "");
    }
    if (text == "/ledoff1")
    {
      ledStatus1 = 1;
      digitalWrite(COOL_5, HIGH);
      digitalWrite(LED_15, HIGH); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led 1 is OFF", "");
    }

     if (text == "/ledoff2")
    {
      ledStatus2 = 1;
      digitalWrite(COOL_18, HIGH);
      digitalWrite(LED_2, HIGH); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led 2 is OFF", "");
    }
     if (text == "/ledoff3")
    {
      ledStatus3 = 1;
      digitalWrite(COOL_19, HIGH);
      digitalWrite(LED_4, HIGH); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led 3 is OFF", "");
    }
    if (text == "/status") 
   {
    checkLedStatus(chat_id);
   }
   

 

   if (text == "/settimer") {
    bot.sendMessage(chat_id, "Please send the start hour (0-23) ps: gmt-3 needs to add 3hrs", "");
     state = 1;
    } else if (state == 1) {
      int startHour = atoi(text.c_str());
      bot.sendMessage(chat_id, "Please send the start minute (0-59)", "");
      state = 2;
      start_time1 = startHour;
    } else if (state == 2) {
      int startMinute = atoi(text.c_str());
      bot.sendMessage(chat_id, "Please send how many hours will lights be on", "");
      state = 3;
      start_time2 = startMinute;
    } else if (state == 3) {
      int endHour = atoi(text.c_str());
      bot.sendMessage(chat_id, "Please send how many minutes will lights be on", "");
      timerCountMs = endHour*60*60*1000;
       state = 4;
    } else if (state == 4) {
      int endMinute = atoi(text.c_str());
      timerCountMs = endMinute*60*1000 + timerCountMs;
      bot.sendMessage(chat_id, "Led timer setup complete", "");
      noTimerSet = 1; //enable ledTimer to work
      timer_lasttime = millis();
      state = 0;
}
    

    if (text == "/start")
    {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Flash Led Bot example.\n\n";
      welcome += "/ledon : to switch the Led ON\n";
      welcome += "/ledoff : to switch the Led OFF\n";
      welcome += "/status : Returns current status of LED\n";
       welcome += "/settimer : Set leds on/off on time";
      
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
