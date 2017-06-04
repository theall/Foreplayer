How to compose a list file (Version 2)

Header Part:
------------
$name= ****
  Zip name of the game.

$version=2
  List file format version. (version 2 is for BrdigeM1 0.5.1 and later)

$default= number
  This number will override the core's default song. (Optional)

$songmax= number
  Specifies the highest song number for the game. (Optional)

$fixed_volume = *** (deprecated)
  Specifies default volume gain when output normalization is disabled.

  * Note: Please do not add fixed_volume to games as below:
    1) need sound re-balancing
    2) driver is not completed such as TaitoF3, SCSP etc.
    3) output volume is unstable
    4) output gets clipped w/o normalization (Valkyrie, Dirtfox, etc.)


Main Part:
----------
$main
  Defines the start of the list.

#Decimal Number + Space
  BridgeM1 recognizes the line as a song number to play.

$Hexadecimal Number + Space
  BridgeM1 recognizes the line as a hexadecimal song number to play.

- Inside optional meta tag
 - time="[mm:]ss[.dd]"
    Defines track length. When Auto-Moving is enabled and it reaches
    the time, automatically moves on to the next song.

    Examples:
    <time="0:05.67">, <time="5.67">, <time="1:12.3">, <time="72.30">,
    <time="3:0">, <time="3:00">, <time="3:00.00">, <time="180">

 - fadeout="yes|no" (0.60a10 and later, optional)
    Enables volume fadeout at the end of the track. Use with a track length setting.
    Regarding a track without this setting,
     - if the track has a time length setting, no fadeout
     - if it doesn't, uses the default fadeout setting

 - fadeoutlength="ss" (0.60a10 and later, optional)
    Specifies lengh of fadeout in seconds. If you omit this, it will use the default
    fadeout length.

&lt; &gt;
  They are replaced with "<" and ">" between "$main" and "$end".

// Comment
  This line is not displayed in the track list window.

$end
  Defines the end of the list.



Thanks to all list contributions!
---------------------------------
  baddy
  Idumi Hatoba
  NK
  Raz
  BD
  eno
  BLADE
  bcass
  M
  Kyuu
  Mihara
  J
  Monyons
  Hiroshi
  Sato
  R
  Penpen
  yakza
  MAZ
  cyada
  Saito
  Min
  huntingforce
  nZero
  Tekkou
  Cratch
  Mappi
  Watabou@Chourou
  Kayama
  Neo Techno Bou
  ZEK.
  CrashTest
  KC
  CycommerAmp
  Yawachary
  Tosh
  Toby
  R.Belmont
  Peter Kutt
  Llama
  Mikasen
  MuramuraNight
  Simon B.
  Knurek
  xfa27
  Skeleton081983
  GolGol
  szk
  a2c-s438
  the Amuse Team 

  and a lot of anonymous contributors
