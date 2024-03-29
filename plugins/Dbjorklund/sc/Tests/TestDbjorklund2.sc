TestDbjorklund2 : UnitTest {

	test_Dbjorklund2 {
		var c= CondVar();
		var r1;
		var f= {|k, n, o, a|
			{Duty.ar(1/a.size/r1, 0, Dbjorklund2(k, n, o, n))}.loadToFloatArray(a.size+0.5/r1, action:{|b|
				this.assertEquals(b.asInteger, a, "k % n % o %".format(k, n, o));
				c.signalOne;
			});
			Server.default.sync;
			c.wait;
		};
		Server.default.bootSync;
		r1= Server.default.sampleRate;

		f.(1, 2, 0, #[2]);
		f.(1, 3, 0, #[3]);
		f.(1, 4, 0, #[4]);
		f.(4, 12, 0, #[3, 3, 3, 3]);
		f.(2, 5, 0, #[2, 3]);
		f.(3, 4, 0, #[1, 1, 2]);
		f.(3, 5, 0, #[2, 2, 1]);
		f.(3, 7, 0, #[2, 2, 3]);
		f.(3, 8, 0, #[3, 3, 2]);
		f.(4, 7, 0, #[2, 2, 2, 1]);
		f.(4, 9, 0, #[2, 2, 2, 3]);
		f.(4, 11, 0, #[3, 3, 3, 2]);
		f.(5, 6, 0, #[1, 1, 1, 1, 2]);
		f.(5, 7, 0, #[2, 1, 2, 1, 1]);
		f.(5, 8, 0, #[2, 1, 2, 1, 2]);
		f.(5, 9, 0, #[2, 2, 2, 2, 1]);
		f.(5, 11, 0, #[2, 2, 2, 2, 3]);
		f.(5, 12, 0, #[3, 2, 3, 2, 2]);
		f.(5, 16, 0, #[3, 3, 3, 3, 4]);
		f.(7, 8, 0, #[1, 1, 1, 1, 1, 1, 2]);
		f.(7, 12, 0, #[2, 1, 2, 2, 1, 2, 2]);
		f.(7, 16, 0, #[3, 2, 2, 3, 2, 2, 2]);
		f.(9, 16, 0, #[2, 1, 2, 2, 2, 1, 2, 2, 2]);
		f.(11, 24, 0, #[3, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2]);
		f.(13, 24, 0, #[2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2]);
		this.assertException({Dbjorklund2(0, 8, 0)}, Error, "k % n % o %".format(0, 8, 0));
		f.(8, 8, 0, #[1, 1, 1, 1, 1, 1, 1, 1]);
		f.(12, 17, 0, #[2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1]);
		f.(12, 27, 0, #[3, 2, 2, 2, 3, 2, 2, 2, 3, 2, 2, 2]);
		f.(41, 50, 0, #[2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1]);

		f.(3, 8, 1, #[3, 2, 3]);
		f.(3, 8, -1, #[2, 3, 3]);
		f.(5, 12, 3, #[2, 2, 3, 2, 3]);
		f.(5, 12, -13, #[3, 2, 2, 3, 2]);
	}
}

//TestDbjorklund2.run
