TestDbjorklund : UnitTest {

	test_Dbjorklund {
		var c= CondVar();
		var r1, r2;
		var f= {|k, n, o, a|
			{Duty.ar(1/n/r1, 0, Dbjorklund(k, n, o, n))}.loadToFloatArray(n+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "ar k % n % o %".format(k, n, o));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
			{Duty.kr(1/n/r2, 0, Dbjorklund(k, n, o, n))}.loadToFloatArray(n+0.5/r2, action:{|b|
				this.assertEquals(b.asInteger, a, "kr k % n % o %".format(k, n, o));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
		};
		Server.default.bootSync;
		r1= Server.default.sampleRate;
		r2= r1/Server.default.options.blockSize;

		f.(1, 2, 0, #[1, 0]);
		f.(1, 3, 0, #[1, 0, 0]);
		f.(1, 4, 0, #[1, 0, 0, 0]);
		f.(4, 12, 0, #[1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0]);
		f.(2, 5, 0, #[1, 0, 1, 0, 0]);
		f.(3, 4, 0, #[1, 1, 1, 0]);
		f.(3, 5, 0, #[1, 0, 1, 0, 1]);
		f.(3, 7, 0, #[1, 0, 1, 0, 1, 0, 0]);
		f.(3, 8, 0, #[1, 0, 0, 1, 0, 0, 1, 0]);
		f.(4, 7, 0, #[1, 0, 1, 0, 1, 0, 1]);
		f.(4, 9, 0, #[1, 0, 1, 0, 1, 0, 1, 0, 0]);
		f.(4, 11, 0, #[1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0]);
		f.(5, 6, 0, #[1, 1, 1, 1, 1, 0]);
		f.(5, 7, 0, #[1, 0, 1, 1, 0, 1, 1]);
		f.(5, 8, 0, #[1, 0, 1, 1, 0, 1, 1, 0]);
		f.(5, 9, 0, #[1, 0, 1, 0, 1, 0, 1, 0, 1]);
		f.(5, 11, 0, #[1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0]);
		f.(5, 12, 0, #[1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0]);
		f.(5, 16, 0, #[1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0]);
		f.(7, 8, 0, #[1, 1, 1, 1, 1, 1, 1, 0]);
		f.(7, 12, 0, #[1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0]);
		f.(7, 16, 0, #[1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0]);
		f.(9, 16, 0, #[1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0]);
		f.(11, 24, 0, #[1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0]);
		f.(13, 24, 0, #[1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0]);
		f.(0, 8, 0, #[0, 0, 0, 0, 0, 0, 0, 0]);
		f.(8, 8, 0, #[1, 1, 1, 1, 1, 1, 1, 1]);
		f.(12, 17, 0, #[1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1]);
		f.(12, 27, 0, #[1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0]);
		f.(41, 50, 0, #[1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1]);

		f.(3, 8, 1, #[0, 0, 1, 0, 0, 1, 0, 1]);
		f.(3, 8, -1, #[0, 1, 0, 0, 1, 0, 0, 1]);
		f.(5, 12, 3, #[1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0]);
		f.(5, 12, -13, #[0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1]);
	}
}

//TestDbjorklund.run
