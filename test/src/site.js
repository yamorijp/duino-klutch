const chakram = require("chakram")
const expect = chakram.expect
const jsend = require("../jsend")
const consts = require("../consts")
const host = consts.host


const heapSchema = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["success"]
    },
    data: {
      properties: {
        vcc: {
          required: true,
          type: "number"
        },
        free_heap: {
          required: true,
          type: "number"
        },
        free_sketch_space: {
          required: true,
          type: "number"
        }
      }
    }
  }
}

const handlersSchema = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["success"]
    },
    data: {
      properties: {
        items: {
          required: true,
          type: "array",
          items: {
            type: "object",
            properties: {
              name: {
                type: "string",
                required: true
              },
              type: {
                type: "string",
                required: true
              },
              group: {
                type: "string",
                required: true
              },
              resource: {
                type: "string",
                required: true
              }
            }
          }
        }
      }
    }
  }
}


describe("Site API", () => {

  if (consts.handlers.site) {
    describe("GET /heap", () => {
      it("should return heap", () => {
        const r = chakram.get(host + '/heap')
        return expect(r).to.have.schema(heapSchema)
      })
    })

    describe("GET /reset", () => {
      it("pass")
    })

    describe("GET /restart", () => {
      it("pass")
    })

    describe("GET /_handlers", () => {
      it("should return success", () => {
        const r = chakram.get(host + '/_handlers')
        return expect(r).to.have.schema(handlersSchema)
      })
    })
  }
})
