// begin readme of YX
//Made with JUCE 5.4.7
This plugin is based off of a Reaktor patch I would frequently make using the XY device and clock divider tool. 
There are clock dividers for the X-shifting, Y-Shifting, and actual note playback, and all MIDI notes are avaliable
for use on the grid. You're going to need to grab your own Quantizer. Remember to "hit record".

Parameters List!

/////////////Globals/////////////

Div: This goes from 1/4 to 1/16th notes, with no triplets in between. Applies to X-Interval, Y-Interval, and Interval.
Can be seen as the Denominator in a musical time signature. 

Velocity: The velocity per note. I am not goint to add per-note velocity. We have other tools for that. 

Gate: percentage of note that is played. Based on Div. If Div is 1/4 notes and gate at 50%, then you can get 1/8th notes. 
If Div is 1/16ths and Gate is still 50%, then you get a 1/32nd. 

///////////Individuals///////////

Midi values per step: This is the main sauce of the grid. They're numbered 0,0 to 3,3 because that's how indexing works in 
anything that isn't MATLAB. Also in a 2-D array, the first index is the row that contains columns, which makes it M[Y][X]. 

X-Interval: This is the clock-divider for how often the X-position moves over on space in the grid. This is the X/Div.

Y-Interval: This is the clock-divider for how often the Y-position moves over on space in the grid. This is the Y/Div.

Interval: This is the clock divider for how often notes are played, and can be seen as the numerator of the time signature,
Interval/Div.The logical processing is that the X and Y move in the background with higher priority, and then a note is 
played if stars align or whatever.

////General Operational Procedures/////

This thing starts the magic on quarter notes, and will play the note currently related to the matrix position. 
If your playhead is off, it'll just wait until the next one. The grid does not reset to 0,0 between starting and stopping.
Oh the thing is an Effect unit, because I can't get the JUCE "Is Synth" override to work for some reason. 
Ableton users: please enable the dinky little UI. No idea why it doesn't work without it. 
FL Studio users: throw it onto a Patcher, and disable the "from FL Studio" thing, and send events to a synth of your choice. 
Just remember to set up the ports. Also, I would suggest to avoid the traditional FL workflow by not working with the looping
and setting a dummy clip far off in the distance, and working in a traditional timeline. Re-recording MIDI in FL requires
an external MIDI driver, so please just send Image-Line a request to add this feature to be as easy as Ableton. 

I hope you enjoy YX. Please send all complaints to ofoot@hotmail.com. I am not a CS Student by degree so please be nice
and constructive. No one likes a complainer without a solution.

Thank you's: To my friend Ben E. who has been the only one who has shown an actual interest in my plugin(s) and went
through the hassle of telling me Fib_Seq doesn't work on his machine. Unlike those wankers on WHY Record. 