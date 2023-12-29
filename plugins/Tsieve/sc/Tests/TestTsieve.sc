TestTsieve : UnitTest {

	test_Tsieve {
		var c= CondVar();
		var r1, r2;
		var f= {|i, p, o, n, a|
			{Tsieve.ar(DC.ar(r1), 0.5, i, p, o)}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Tsieve.ar(DC.ar(r1), DC.ar(0.5), i, p, o)}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar2 i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Tsieve.ar(r1, DC.ar(0.5), i, p, o)}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar3 i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Tsieve.ar(DC.kr(r1), 0.5, i, p, o)}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar4 i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Tsieve.kr(r2, 0.5, i, p, o)}.loadToFloatArray(n+0.5/r2, action:{|b|
				this.assertEquals(b.asInteger, a, "kr i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
		};
		var g= {|i1, p1, o1, op, i2, p2, o2, n, a|
			{Tsieve.ar(DC.ar(r1), 0.5, i1, p1, o1).perform(op, Tsieve.ar(DC.ar(r1), 0.5, i2, p2, o2))}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar op % n %".format(op, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Tsieve.kr(r2, 0.5, i1, p1, o1).perform(op, Tsieve.kr(r2, 0.5, i2, p2, o2))}.loadToFloatArray(n+0.5/r2, action:{|b|
				this.assertEquals(b.asInteger, a, "kr op % n %".format(op, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
		};
		Server.default.bootSync;
		r1= Server.default.sampleRate;
		r2= r1/Server.default.options.blockSize;

		f.(#[0, 1, 3], 4, 0, 4, #[1, 1, 0, 1]);
		f.(#[0, 1, 3], 4, 0, 8, #[1, 1, 0, 1, 1, 1, 0, 1]);
		f.(#[0, 1, 3], 4, 0, 9, #[1, 1, 0, 1, 1, 1, 0, 1, 1]);
		f.((1..10), 4, 0, 9, #[0, 1, 1, 1, 0, 1, 1, 1, 0]);
		f.(#[0, 3, 4, 9], 10, 0, 10, #[1, 0, 0, 1, 1, 0, 0, 0, 0, 1]);
		f.(#[4, 5, 6, 8], 10, 0, 10, #[0, 0, 0, 0, 1, 1, 1, 0, 1, 0]);
		f.(#[0, 1, 3], 4, 1, 8, #[1, 1, 1, 0, 1, 1, 1, 0]);
		f.(#[0, 1, 3], 4, -1, 8, #[1, 0, 1, 1, 1, 0, 1, 1]);
		f.(#[0, 3], 4, 0, 16, #[1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1]);
		f.(#[0, 3], 4, -1, 12, #[0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1]);
		f.(#[0, 3], 4, 1, 12, #[1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0]);
		f.(#[0, 3], 4, 1.8, 12, #[1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0]);
		g.(#[0, 1, 7], 8, 0, '&', #[1, 3], 5, 0, 40, #[0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0]);
		g.(#[1, 3], 4, 0, '|', #[0, 1, 3], 4, 0, 8, #[1, 1, 0, 1, 1, 1, 0, 1]);
	}
}

//TestTsieve.run
