const chakram = require("chakram")
const expect = chakram.expect
const jsend = require("../jsend")
const consts = require("../consts")
const host = consts.host

const rgbSchema = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["success"]
    },
    data: {
      properties: {
        red: {
          required: true,
          type: "number"
        },
        green: {
          required: true,
          type: "number"
        },
        blue: {
          required: true,
          type: "number"
        }
      }
    }
  }
}


describe("LED API", () => {

  after(() => {
    if (consts.handlers.matrix) chakram.get(host + "/led/matrix/clear")
    if (consts.handlers.rgb) chakram.get(host + "/led/rgb/off")
    if (consts.handlers.fourdigit) chakram.get(host + "/led/4digit/clear")
    chakram.wait()
  })

  if (consts.handlers.matrix) {
    describe("GET /led/matrix/clear", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/matrix/clear");
        return expect(r).to.have.schema(jsend.success);
      })
    })

    describe("GET /led/matrix/message", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/matrix/message?data=hello")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/led/matrix/message?data=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should support n query", () => {
        const r = chakram.get(host + "/led/matrix/message?data=world&n=a")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should support instant query", () => {
        const r = chakram.get(host + "/led/matrix/message?data=hoge&instant=a")
        return expect(r).to.have.schema(jsend.fail)
      })
    })

    describe("GET /led/matrix/next", () => {
      it("should return success", () => {
        chakram.get(host + "/led/matrix/message?data=text1");
        chakram.get(host + "/led/matrix/message?data=text2");
        const r = chakram.get(host + "/led/matrix/next");
        return expect(r).to.have.schema(jsend.success);
      })
    })

    describe("GET /led/matrix/columns", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/matrix/columns?data=0")
        expect(r).to.have.schema(jsend.success)

        const r2 = chakram.get(host + "/led/matrix/columns?data=ffff0000")
        expect(r2).to.have.schema(jsend.success)

        return chakram.wait()
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/led/matrix/columns?")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should data query conform hex format", () => {
        const r = chakram.get(host + "/led/matrix/columns?data=0h")
        return expect(r).to.have.schema(jsend.fail)
      })
    })
  }

  if (consts.handlers.rgb) {
    describe("GET /led/rgb/on", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/rgb/on")
        return expect(r).to.have.schema(rgbSchema)
      })

      it("should support data query", () => {
        const r = chakram.get(host + "/led/rgb/on?data=ff00ff")
        return expect(r).to.have.schema(rgbSchema)
      })

      it("should data query conform hex format", () => {
        const r = chakram.get(host + "/led/rgb/on?data=1.2")
        return expect(r).to.have.schema(jsend.fail)
      })
    })

    describe("GET /led/rgb/off", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/rgb/off")
        return expect(r).to.have.schema(rgbSchema)
      })
    })

    describe("GET /led/rgb/state", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/rgb/state")
        return expect(r).to.have.schema(rgbSchema)
      })
    })
  }

  if (consts.handlers.fourdigit) {
    describe("GET /led/4digit/clear", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/4digit/clear")
        return expect(r).to.have.schema(jsend.success)
      })
    })

    describe("GET /led/4digit/number", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/4digit/number?data=1234")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should support leading_zero query", () => {
        const r = chakram.get(host + "/led/4digit/number?data=123&leading_zero=1")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should support brightness query", () => {
        const r = chakram.get(host + "/led/4digit/number?data=123&brightness=4")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/led/4digit/number?data=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should data query conform 0 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/number?data=10000")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should leading_zero query conform 1 or 0", () => {
        const r = chakram.get(host + "/led/4digit/number?data=100&leading_zero=2")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should brightness query conform 0 to 7", () => {
        const r = chakram.get(host + "/led/4digit/number?data=100&brightness=8")
        return expect(r).to.have.schema(jsend.fail)
      })
    })

    describe("GET /led/4digit/segments", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/4digit/segments?data=ff00ff00")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should support brightness query", () => {
        const r = chakram.get(host + "/led/4digit/segments?data=ff00ff00&brightness=4")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should require data query", () => {
        const r = chakram.get(host + "/led/4digit/segments?data=")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should data query conform hex format", () => {
        const r = chakram.get(host + "/led/4digit/segments?data=101g")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should brightness query conform 0 to 7", () => {
        const r = chakram.get(host + "/led/4digit/segments?data=ff&brightness=8")
        return expect(r).to.have.schema(jsend.fail)
      })
    })

    describe("GET /led/4digit/counting", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/4digit/counting?from=10&to=99&step=2")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should from query conform 0 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/counting?from=10000")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should to query conform 0 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/counting?to=10000")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should step query conform 1 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/counting?step=0")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should brightness query conform 0 to 7", () => {
        const r = chakram.get(host + "/led/4digit/counting?brightness=-1")
        return expect(r).to.have.schema(jsend.fail)
      })
    })

    describe("GET /led/4digit/roll", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/led/4digit/roll?min=2&max=12")
        return expect(r).to.have.schema(jsend.success)
      })

      it("should min query conform 0 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/roll?min=10000")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should max query conform 0 to 9999", () => {
        const r = chakram.get(host + "/led/4digit/roll?max=10000")
        return expect(r).to.have.schema(jsend.fail)
      })

      it("should brightness query conform 0 to 7", () => {
        const r = chakram.get(host + "/led/4digit/roll?brightness=-1")
        return expect(r).to.have.schema(jsend.fail)
      })
    });
  }
})
