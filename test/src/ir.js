const chakram = require("chakram")
const expect = chakram.expect
const jsend = require("../jsend")
const consts = require("../consts")
const host = consts.host

describe("IR API", () => {

  if (consts.handlers.ir || consts.handlers.irx) {
    describe("GET /ir/transmitter/raw", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/ir/transmitter/raw?data=1234,567,890,123");
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/ir/transmitter/raw?data=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should data query conform raw data format", () => {
        const r = chakram.get(host + "/ir/transmitter/raw?data=abcd")
        expect(r).to.have.schema(jsend.fail)

        const r2 = chakram.get(host + "/ir/transmitter/raw?data=1234, 300")
        expect(r2).to.have.schema(jsend.success)

        return chakram.wait()
      })

      it("should support hz query", () => {
        const r = chakram.get(host + "/ir/transmitter/raw?data=1234,567,890,123&hz=32")
        return expect(r).to.have.schema(jsend.success)
      })
    })

    describe("GET /ir/transmitter/send", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=ff0123&type=3&data=ffe01f&bits=32")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=&type=3&data=ffe01f&bits=32")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should require type query", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=ff0123&type=&data=ffe01f&bits=32")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should require bits query", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=ff0123&type=3&data=ffe01f&bits=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should data query conform hex format", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=hoge&type=3&data=ffe01f&bits=32")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should type query conform 1 to 40", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=ff0123&type=0&data=ffe01f&bits=32")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should bits query conform 1 to 1000", () => {
        const r = chakram.get(host + "/ir/transmitter/send?data=ff0123&type=0&data=ffe01f&bits=0")
        return expect(r).to.have.schema(jsend.fail)
      })
    })
  }

  if (consts.handlers.irx) {
    describe("GET /ir/transmitter/command", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/ir/transmitter/command?data=koizumi_cl_off")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require command query", () => {
        const r = chakram.get(host + "/ir/transmitter/command?data=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should command query match any of options", () => {
        const r = chakram.get(host + "/ir/transmitter/command?data=hoge")
        return expect(r).to.have.schema(jsend.fail)
      })
    })
  }
})
