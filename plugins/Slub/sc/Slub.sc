// /fO 050202  in honour of uk laptopduo slub (www.slub.org)

//careful with the spike!

Slub : UGen {

	*ar {|trig= 0.0, spike= 4.04|
		^this.multiNew('audio', trig, spike)
	}
	
	*kr {|trig= 0.0, spike = 4.04|
		^this.multiNew('control', trig, spike)
	}
	
	*bang {|spike= 4.04|
		^this.new.bang;
	}
	
	bang {|spike= 4.04|
	
		var
				x
						;
				x
		=
				round
						(
				2
		.
				pow
						(
		spike
		)
				)
						;
				for
		(
				0
						,
				x
		,{
						|
				i
		|
				for
						(
				0
		,
				x
						,
				{
		|
				j
						|
				for
		(
				0
						,
				x
		,
				{
						|
			k
		|
				for
						(
				0
		,
				x
						,
		{
		|
		l
						|
						l
		.
				mod
						(
				3.1415
		)
				;
						}
			)
		}
			)
				}
			)
		}
		)
				}
}
