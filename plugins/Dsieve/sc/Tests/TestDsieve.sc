TestDsieve : UnitTest {

	test_Dsieve {
		var c= CondVar();
		var r1, r2;
		var f= {|i, p, o, n, a|
			{Duty.ar(1/n/r1, 0, Dsieve(i, p, o, n))}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Duty.kr(1/n/r2, 0, Dsieve(i, p, o, n))}.loadToFloatArray(n+0.5/r2, action:{|b|
				this.assertEquals(b.asInteger, a, "kr i % p % o % n %".format(i, p, o, n));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
		};
		var g= {|i1, p1, o1, op, i2, p2, o2, n, a|
			{Duty.ar(1/n/r1, 0, Dsieve(i1, p1, o1, n).perform(op, Dsieve(i2, p2, o2, n)))}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar op %%".format(op));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Duty.kr(1/n/r2, 0, Dsieve(i1, p1, o1, n).perform(op, Dsieve(i2, p2, o2, n)))}.loadToFloatArray(n+0.5/r2, action:{|b|
				this.assertEquals(b.asInteger, a, "kr op %".format(op));
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
		f.(#[0, 3], {Ddup(8, Dseq(#[4, 2], inf))}, 0, 16, #[1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0]);
		f.(#[0, 3], 4, {Ddup(4, Dseq(#[0, -1, 1], inf))}, 12, #[1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0]);
		g.(#[0, 1, 7], 8, 0, '&', #[1, 3], 5, 0, 40, #[0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0]);
		g.(#[1, 3], 4, 0, '|', #[0, 1, 3], 4, 0, 8, #[1, 1, 0, 1, 1, 1, 0, 1]);
	}
}

//TestDsieve.run
