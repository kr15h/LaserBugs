//for recording
 Server.internal.options.device = "Soundflower (16ch)";
    Server.local.options.device = "Soundflower (16ch)";

tips about screen recording with sound
"http://www.cnet.com/how-to/record-your-computers-screen-with-audio-on-a-mac/"
//first thing to do
s.makeWindow
s.boot

(
OSCdef('collisionLister', {
	arg msg;
	Synth('test3', [pan:msg[1],freq:msg[2]]);
},"/collision");
)

// for rithm
(
SynthDef('test3', {
	arg pan = 0, freq = 200;
	var osc = LFPulse.ar(freq,0,0.5,0.2);
	var env = EnvGen.kr(Env.perc(0, 0.08),doneAction:2);
	Out.ar(0, Pan2.ar(osc*env, pan, 1));
}).store
)












//sound definition
(
SynthDef('test1', {
	arg pan = 0;
	var osc = Impulse.ar(40,0,1);
	var env = EnvGen.kr(Env.perc(0, 0.2),doneAction:2);
	Out.ar(0, Pan2.ar(osc*env, pan, 1));
}).store
)
//check
Synth('test1')

(
SynthDef('test2', {
	arg pan = 0, freq = 200;
	var osc = LFPulse.ar(200,0,0.5,0.15);
	var env = EnvGen.kr(Env.perc(0, 0.05),doneAction:2);
	Out.ar(0, Pan2.ar(osc*env, pan, 1));
}).store
)
Synth('test2')



play{LFPulse.ar(200,0,0.5,0.1)}

//check the address to talk to from Processing
NetAddr.localAddr

(
OSCdef('collisionLister', {
	arg msg;
	Synth('test2', [pan:msg[1]], );
},"/collision");
)


// for rithm
(
SynthDef('test3', {
	arg pan = 0, freq = 200;
	var osc = LFPulse.ar(freq,0,0.5,0.2);
	var env = EnvGen.kr(Env.perc(0, 0.08),doneAction:2);
	Out.ar(0, Pan2.ar(osc*env, pan, 1));
}).store
)
Synth('test3')

(
SynthDef('noi', {
	arg pan = 0, freq = 200;
	var osc1 = ClipNoise.ar(0.5);
	var osc2 = GrayNoise.ar(0.5);
	var env = EnvGen.kr(Env.perc(0, 0.06),doneAction:2);
	Out.ar(0, Pan2.ar(osc1*env+osc2*env, pan, 1));
}).store
)
(
SynthDef('noi', {
	arg pan = 0, freq = 200;
	var osc1 = ClipNoise.ar(0.5);
	var osc2 = GrayNoise.ar(0.5);
	var env = EnvGen.kr(Env.perc(0, 0.07),doneAction:2);
	var fx = FreeVerb.ar(osc1*env+osc2*env, 0.4, 1, 0.01, 1);
	Out.ar(0, Pan2.ar(fx, pan, 1));
}).store
)
Synth('noi')

(
SynthDef('imp', {
	arg pan = 0, freq = 200;
	var osc1 = Impulse.ar(1,0,1);
	var env = EnvGen.kr(Env.perc(0, 0.06),doneAction:2);
	Out.ar(0, Pan2.ar(osc1*env, pan, 1));
}).store
)
Synth('imp')

(
OSCdef('collisionLister', {
	arg msg;
	Synth('test3', [pan:msg[1],freq:msg[2]]);
},"/collision");
)



SynthDef("help-Pan2", { Out.ar(0, Pan2.ar(PinkNoise.ar(0.4), FSinOsc.kr(2), 0.3)) }).play;